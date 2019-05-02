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

// Frequency of preemption 100Hz is 100 times per second
#define HZ 100

struct itimerval timer;

// use the signal to force the current thread to yield
void handlerFunction(int signum) {
  uthread_yield();
}

void preempt_disable(void)
{
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = SIG_IGN;
  sigaction(SIGVTALRM, &sa, NULL);
}

void preempt_enable(void)
{
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = &handlerFunction;
  sigaction(SIGVTALRM, &sa, NULL);
}


// reference of how to use sigaction and timer together
// http://www.informit.com/articles/article.aspx?p=23618&seqNum=14
void preempt_start(void)
{
  // set timer to go off 100 times per second
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 1000000 / HZ;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 1000000 / HZ;
  setitimer(ITIMER_VIRTUAL, &timer, NULL);

  preempt_enable();
}
