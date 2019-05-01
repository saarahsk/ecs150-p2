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
};

uthread_t next_thread_id = 1;
queue_t queue = NULL;
tcb_t active_thread = NULL;

void uthread_yield(void)
{
  /* TODO Phase 2 */
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
  thread->state = READY;
  thread->stack = NULL; // we don't know :(

  int result = queue_enqueue(queue, thread);
  if (result == -1) {
    fprintf(stderr, "Error enqueuing main thread");
    return NULL;
  }

  return thread;
}

int uthread_create(uthread_func_t func, void *arg)
{
  int first = 0;
  if (queue == NULL) {
    first = 1;
    queue = queue_create();
    if (queue == NULL) {
      return -1;
    }
  }

  // create the user thread and enqueue
  tcb_t thread = uthread_create_helper(func, arg);
  int result = queue_enqueue(queue, thread);
  if (result == -1) {
    fprintf(stderr, "Error enqueuing new thread");
    return -1;
  }

  if (first) {
    tcb_t main_thread = first_initialization();
    uthread_ctx_switch(&main_thread->context, &thread->context);
  }

  return thread->tid;
}

void uthread_exit(int retval)
{
  /* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
  /* TODO Phase 2 */
  /* TODO Phase 3 */
  return 0;
}
