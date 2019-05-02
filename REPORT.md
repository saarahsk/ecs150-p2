Project 2 had us create a threading library that users could use to launch their own threads within
their applications. The project was broken up into four phases that we completed in order.

Phase 1
For phase 1, we implemented a FIFO queue library. The library implementation was pretty
straightforward considering that we had implemented FIFO queues before in our previous classes. The
hardest part of this phase was to ensure that we had all the pointers in the queue correct. Since we
had a head pointer as well as a tail pointer, we had to make sure that the two pointers were always
correct whenever we did any operation on them. We found a few bugs while implementing this phase but
were able to fix them with the help of gdb. Unit testing the queue really helped as well to make
sure that we didn't introduce other bugs while doing the fixes.

The other part of phase 1 that was a bit tricky was the queue iterate function. The way that it was
sending data back to the user by passing in a function to call on each element was very strange. It
took a long time to get that right.

Phase 2
Phase 2 was probably the most challenging part of the project. It's where the actual threading
library took form. The first thing we did was to create a thread control block data structure so
that we could keep track of all the state of the thread. We have a variable called active_thread
that constantly points to the thread that is currently running. On the first thread create, we
create a thread control block for the currently running thread and assign it to the active thread
variable. We then create a thread control block for the thread that the user requested to create and
enqueue it on the queue of threads to be run. Since threads need to return to the caller before the
child thread is executed, we simply returned back to the caller. The child thread would get
scheduled later and the main thread would give up execution when it called join later on.

The next step of this phase was to write thread yield. Thread yield simply places the current active
thread on the end of the queue of active threads and dequeues the next thread to be run. Then, we
perform the context switch and things magically start running properly. We found this phase to be
the most challenging because it took a while to get used to the ideas behind the context file. In
all programs we've written before, execution always returns back to the function that called it.
This is the first time that execution goes to a completely different location in the program. It
took a really long time and lots of piazza searching to understand how to get the main thread to
actually get scheduled onto the active thread queue and give up control to the thread that just got
scheduled.

Phase 3
Phase 3 had us implement join properly. This sounded easy considering how easy the join function in
phase 2 was. However, it turned out to be deceptively difficult. After phase 2, this was the most
difficult phase in the project. This phase had us introduce another queue in our program that stored
the threads that were in a zombie state. Once a thread had called exit, it was removed from the
active thread queue, its state turned to a zombie, and enqueued onto the zombie queue. Now that we
had two thread queues, the join function had to be modified to look at whether the thread the user
wanted to join on was currently in the active queue or in the zombie queue. If it was in the active
queue, then the joining thread had to wait for finish. In that case, we yielded to another thread.
However, if it was in the zombie thread, then it could be reaped by the calling thread. All this was
done in a while loop that we only broke out of once we actually finished reaping a thread. Doing so
meant that the joining thread would keep trying to join until it was successfully able to pull the
done thread out of the zombie queue. Eventually, it would yield and give other threads a chance to
do some work and come back and try again.

We ran into some pretty nasty bugs here that took a long time to solve. The first was to make sure
that we were restarting the loop by calling continue once we yielded. After the yield, we would
eventually get scheduled back but starting with where we yielded from. It took a long time to
realize that. It's important to do this because if we don't, then we would assume we had found the
correct thread in the zombie queue when we hadn't. We needed to continue to go back and try the
search again. The next bug that was pretty tricky to find here was where we needed to break out of
the loop after all the joining was done. We broke after we had successfully reaped at least one
thread since a thread can only reap one thread at one time. This was just really hard to wrap our
minds around at first because there are all these threads coming in and out and the program is
jumping around all over the place depending on which thread gets scheduled.

Phase 4
Phase 4 had us implement pre-emption. Without it, a thread could never call yield and no other
threads would ever run. This was implemented by using alarm signals so that the operating system
would interrupt our program and run a signal handler. Using the sigaction function, we registered a
signal handler that would be called 100 times a second. Each time the signal handler was called, we
would manually call thread yield so as to give another thread a chance to run. The danger here is
that the signal handler might be called when the library itself is running and managing threads. We
had to enable and disable pre-emption at various points in our code so as to make sure that the
threading library wouldn't get interrupted when it was doing important operations on the threading
data structures. We placed the pre-emption enable and disable functions around wherever we were
modifying the global threading state (e.g. enqueuing, dequeing, etc).

Testing
We tested our code using the test programs that are in the test directory. All the tests work and we
added a few of our own for the queue part of the program as well as testing joins. We could have
definitely used more testing but we didn't have time to write too many more test programs.
