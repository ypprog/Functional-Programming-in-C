.PHONY: all clean

CC = gcc
CFLAGS = -O0 -g -Wall -Werror -std=gnu99

all: reverse mergesort test-functional

reverse: reverse.o

mergesort: mergesort.o

test-functional: functional.o gc.o list.o test-functional.o

clean:
	rm -f *.o reverse mergesort test-functional
