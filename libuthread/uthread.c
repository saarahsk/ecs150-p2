#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

typedef struct tcb* tcb_t;

// this enum lets us keep track of the current state of the thread
enum ThreadState { READY, RUNNING, BLOCKED, ZOMBIE };

//create a struct that will hold each thread's execution state and details
struct tcb {
  uthread_t tid;
  enum ThreadState state;
  uthread_ctx_t context;
  void* stack;
  int retval;
};

uthread_t next_thread_id = 1;

queue_t active_queue = NULL;
queue_t zombie_queue = NULL;

tcb_t active_thread = NULL;

int find_by_id(void *data, void *arg) {
  tcb_t thread = (tcb_t)data;
  int* tid = (int*)arg;

  if (thread->tid == *tid) {
    // found our thread, so stop iteration
    return 1;
  }

  return 0;
}

void uthread_yield(void)
{
  preempt_disable();

  // blocked threads shouldn't be assumed to be ready
  if (active_thread->state != BLOCKED) {
    active_thread->state = READY;
  }

  // place current thread at the end of the active threads
  int result = queue_enqueue(active_queue, active_thread);
  if (result == -1) {
    fprintf(stderr, "Error: failed enqueuing active queue back onto thread queue\n");
    return;
  }

  // take the first thread off of the currently pending threads
  tcb_t thread = NULL;
  result = queue_dequeue(active_queue, (void**)&thread);
  if (result == -1) {
    fprintf(stderr, "Error: failed dequeing when yielding\n");
    return;
  }

  tcb_t prev_thread = active_thread;
  active_thread = thread;
  active_thread->state = RUNNING;
  preempt_enable();

  // context switch to the new thread
  uthread_ctx_switch(&prev_thread->context, &active_thread->context);
}

uthread_t uthread_self(void)
{
  return active_thread->tid;
}

tcb_t uthread_create_helper(uthread_func_t func, void* arg) {
  // create enough memory for the thread
  tcb_t thread = malloc(sizeof(struct tcb));
  thread->tid = next_thread_id++;
  thread->state = READY;
  thread->retval = -1;

  thread->stack = uthread_ctx_alloc_stack();
  if (thread->stack == NULL) {
    // make sure we don't have any memory leaks on errors
    free(thread);
    return NULL;
  }

  int result = uthread_ctx_init(&thread->context, thread->stack, func, arg);
  if (result == -1) {
    // make sure we don't have any memory leaks on errors
    uthread_ctx_destroy_stack(thread->stack);
    return NULL;
  }

  return thread;
}

tcb_t first_initialization() {
  // create a tcb for the main thread
  tcb_t thread = malloc(sizeof(struct tcb));
  thread->tid = 0;
  thread->state = RUNNING;
  thread->stack = NULL; // we don't know how to get a stack pointer :(
  thread->retval = -1;

  // don't enqueue the main (active) thread or we will eventually try to swap with ourselves
  return thread;
}

int uthread_create(uthread_func_t func, void *arg)
{
  if (active_queue == NULL) {
    active_queue = queue_create();
    if (active_queue == NULL) {
      return -1;
    }

    zombie_queue = queue_create();
    if (zombie_queue == NULL) {
      return -1;
    }

    tcb_t main_thread = first_initialization();
    active_thread = main_thread;
  }

  // create the user thread and enqueue
  tcb_t thread = uthread_create_helper(func, arg);

  preempt_disable();
  int result = queue_enqueue(active_queue, thread);
  if (result == -1) {
    fprintf(stderr, "Error: couldn't enqueue new thread\n");
    return -1;
  }
  preempt_enable();

  return thread->tid;
}

void uthread_exit(int retval)
{
  int length = queue_length(active_queue);
  if (length == 0) {
    fprintf(stderr, "Error: called uthread_exit without a thread queue\n");
    return;
  }

  // the current thread is done, so it is a zombie and needs to wait for someone to join it
  preempt_disable();
  active_thread->state = ZOMBIE;
  active_thread->retval = retval;
  int result = queue_enqueue(zombie_queue, active_thread);
  if (result == -1) {
    fprintf(stderr, "Error: failed enqueuing exited thread onto zombie thread\n");
    return;
  }

  // get the next runnable thread and run it
  result = queue_dequeue(active_queue, (void**)&active_thread);
  if (result == -1) {
    fprintf(stderr, "Error: no more threads to dequeue\n");
    return;
  }

  active_thread->state = RUNNING;
  preempt_enable();

  struct tcb thread; // current thread is dying so don't care about its context anymore
  uthread_ctx_switch(&thread.context, &active_thread->context);
}

int uthread_join(uthread_t tid, int *retval)
{
  if (tid == 0) {
    fprintf(stderr, "Error: cannot join the main thread\n");
    return -1; // we cannot join the main thread
  }

  if (active_thread->tid == tid) {
    fprintf(stderr, "Error: cannot join your own thread\n");
    return -1; // we cannot join ourselves
  }

  while (1) {
    tcb_t thread = NULL;

    // check in the active queue for the thread
    queue_iterate(active_queue, find_by_id, &tid, (void**)&thread);
    if (thread != NULL) {
      // thread is still running, current thread is blocked on it finishing
      active_thread->state = BLOCKED;
      uthread_yield();
      continue; // ensure that when we get rescheduled, we try the search again
    }

    // check in the active queue for the thread
    queue_iterate(zombie_queue, find_by_id, &tid, (void**)&thread);
    if (thread == NULL) {
      // tid doesn't exist in active queue as well as zombie queue?
      fprintf(stderr, "Error: cannot join a non-existant thread\n");
      return -1; // we cannot join a non-existent thread
    }

    if (retval != NULL) {
      *retval = thread->retval;
    }

    preempt_disable();
    int result = queue_delete(zombie_queue, thread);
    if (result == -1) {
      fprintf(stderr, "Error: cannot delete from zombie queue\n");
      return -1;
    }

    free(thread);
    active_thread->state = READY;
    uthread_yield();

    preempt_enable();
    break; // don't try to continue to search for zombie threads once we have joined all
  }

  return 0;
}
