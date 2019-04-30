lib := libuthread.a

all: $(lib)

$(lib) : queue.o
	ar rcs $(lib) queue.o

all: queue.o
	gcc -o queue -Wall -Werror  -Werror -Wno-deprecated queue.o

queue.o: queue.c
	gcc -c queue.c





## TODO: Phase 1.1

# all: queue.o
# 	gcc -o queue -Wall  -Werror -Wno-deprecated queue.o
#
# queue.o: queue.c
# 	gcc -c queue.c
#
# clean:
# 	rm queue *.o
