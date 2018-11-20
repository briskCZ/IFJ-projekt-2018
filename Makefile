
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -Wextra -g

all: scanner.c scanner.h string.c string.h trash.h trash.c parser.c expr_parser.c expr_parser.h stack.c stack.h int_stack.c int_stack.h
	${CC} ${CFLAGS} *.c -o all

string: string.c string.h
	${CC} ${CFLAGS} string.c -o string

scanner: scanner.c scanner.h
	${CC} ${CFLAGS} scanner.c -o scanner

clean:
	rm all string
#
