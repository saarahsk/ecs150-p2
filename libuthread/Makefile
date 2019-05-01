lib := libuthread.a

all: $(lib)

$(lib): queue.o
	ar rcs $(lib) queue.o

queue.o: queue.c
	gcc -c queue.c

clean:
	rm *.o