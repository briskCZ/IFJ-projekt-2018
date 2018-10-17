
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm


test_table: unit_symtable.c symtable.c string.c symtable.h scanner.h string.h
	${CC} ${CFLAGS} unit_symtable.c symtable.c string.c symtable.h scanner.h string.h -o test_symtable

clean:
	rm -f *.o *.out $(PROGS)
#
