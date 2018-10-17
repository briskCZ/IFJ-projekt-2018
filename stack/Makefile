
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

test_stack: stack.c stack.h unit_stack.c 
	${CC} ${CFLAGS} stack.c stack.h unit_stack.c -o test_stack

clean:
	rm -f *.o *.out $(PROGS)
#
