
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm


string: string.c string.h
	${CC} ${CFLAGS} string.c -o string

scanner: scanner.c scanner.h
	${CC} ${CFLAGS} scanner.c -o scanner

all: scanner.c scanner.h string.c string.h
	${CC} ${CFLAGS} *.c -o all
clean:
	rm all string
#
