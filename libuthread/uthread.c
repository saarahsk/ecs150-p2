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
enum threadState { READY, RUNNING, BLOCKED, ZOMBIE };

//create a struct that will hold each thread's execution content

struct tcb {
  uthread_t tid; // this is the thread id
  enum threadState; // the current state of the thread
  uthread_ctx_t context; //the context of the thread
  void *stack; // the stack belonging to the thread
};

// this function initializes the uthread library by registering
// the single execution flow of the application as the main thread.

int uthread_init() {
  // create enough memory for the thread
  tcb_t main = malloc(sizeof(struct tcb));

  // initialize the main thread
  main->tid = 0;
  main->threadState = READY;
  main->content = NULL;



}

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
	/* TODO Phase 2 */
	/* TODO Phase 3 */
}
