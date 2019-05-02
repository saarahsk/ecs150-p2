#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100


// need timer to fire off 100 times per second, so 1 sec/100
// which is 100000/HZ

// use the signal to force the current thread to yield
void handlerFunction(int signum) {
  // uthread_yield();
  static int count = 0;
 printf ("timer expired %d times\n", ++count);
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
}


// reference of how to use sigaction and timer together
// http://www.informit.com/articles/article.aspx?p=23618&seqNum=14
void preempt_start(void)
{
  struct sigaction sa;
  struct itimerval timer;

  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = &handlerFunction;

  sigaction(SIGVTALRM, &sa, NULL);

  // set timer to go off 100 times per second
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 100000/HZ;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 100000/HZ;

  settimer(ITIMER_VIRTUAL, &timer, NULL);


}

int main() {
  preempt_start();
}
