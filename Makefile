
PARSER=expr_parser.c expr_parser.h parser.c parser.h
AUX=int_stack.c int_stack.h string.c string.h token_array.c token_array.h
DATA=param_array.c
GEN=ins_list.c ins_list.h generator.h generator.c
FLAGS= -Wall -std=c99 -pedantic -Wextra -g

main: $(PARSER) $(GEN) ret_vals.h scanner.c scanner.h symtable.c symtable.h $(DATA) $(AUX)
	gcc $(FLAGS) *.c -o main

tarr: token_array.c token_array.h scanner.c scanner.h string.c string.h parser.c parser.h
	gcc $(FLAGS) token_array.c scanner.c string.c parser.c -o tarr
