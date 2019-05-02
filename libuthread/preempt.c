#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  uthread_yield();
}

void preempt_disable(void)
{
  struct sigaction ignore;

  // the signal that we want to block

  ignore.sa_handler = SIG_IGN;
  sigaction(SIGVTALRM, &ignore, NULL);

}

void preempt_enable(void)
{
    struct sigaction signal;

    signal.sa_handler = &handlerFunction;
    sigaction(SIGVTALRM, &signal, NULL); 
}


// reference of how to use sigaction and timer together
// http://www.informit.com/articles/article.aspx?p=23618&seqNum=14
void preempt_start(void)
{
  struct sigaction sa;
  struct itimerval timer;

  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = &handlerFunction;

  sigaction(SIGVTALRM, &sa, NULL);

  // set timer to go off 100 times per second
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 1000000 / HZ;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 1000000 / HZ;

  setitimer(ITIMER_VIRTUAL, &timer, NULL);
}
