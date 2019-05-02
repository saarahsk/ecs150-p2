lib := libuthread.a

flags := -Wall -pedantic -g -O0

# Mac OS X has deprecated
ifeq ($(shell uname -s), Linux)
flags += -Werror
endif

objects := context.o preempt.o queue.o uthread.o

all: $(lib)

$(lib): $(objects)
	ar rcs $(lib) $(objects)

%.o: %.c
	gcc -c $(flags) -o $@ $<

clean:
	rm -f $(objects) $(lib)
