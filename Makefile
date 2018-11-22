
PARSER=expr_parser.c expr_parser.h parser.c parser.h 
AUX=int_stack.c int_stack.h string.c string.h 
DATA=param_array.c
GEN=ins_list.c ins_list.h generator.h generator.c

main: $(PARSER) $(GEN) ret_vals.h scanner.c scanner.h symtable.c symtable.h $(DATA) $(AUX)
	gcc -Wall $(PARSER) $(GEN) ret_vals.h scanner.c scanner.h symtable.c symtable.h $(DATA) $(AUX) -o main
