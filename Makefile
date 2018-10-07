
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm


string: string.c string.h
	${CC} ${CFLAGS} string.c -o string

clean:
	rm -f *.o *.out $(PROGS)
#
