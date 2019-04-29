# Target library
# lib := libuthread.a
#
# all: $(lib)
#
# ## TODO: Phase 1.1

all: queue.o
	gcc -o queue -Wall  -Werror -Wno-deprecated queue.o

sshell.o: queue.c
	gcc -c queue.c

clean:
	rm queue *.o
