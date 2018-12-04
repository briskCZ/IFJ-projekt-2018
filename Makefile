PARSER=expr_parser.c expr_parser.h parser.c parser.h 
AUX=int_stack.c int_stack.h string.c string.h 
DATA=token_array.c token_array.h
GEN=ins_list.c ins_list.h generator.h generator.c

main: $(PARSER) $(GEN) ret_vals.h scanner.c scanner.h symtable.c symtable.h $(DATA) $(AUX)
	gcc $(PARSER) $(GEN) ret_vals.h scanner.c scanner.h symtable.c symtable.h $(DATA) $(AUX) -o main
	
test:
	sh ./test.sh