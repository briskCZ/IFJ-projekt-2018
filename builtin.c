


void builtinPrint()
{
	int max_temp;
	printf("DEFVAR %s@$%d\n", ramec(), unique_num++);
	printf("TYPE %s@$%d %s\n", ramec(), unique_num-1, ramec(), (void*)list->first->data->adr1);

	printf("JUMPIFEQ $L%d  string@int %s@$%d\n", temp_label++, ramec(), unique_num-1);
	printf("JUMPIFEQ $L%d  string@float %s@$%d\n", temp_label++, ramec(), unique_num-1);
	printf("JUMPIFEQ $L%d  string@string %s@$%d\n", temp_label++, ramec(), unique_num-1);
	printf("JUMPIFEQ $L%d  string@nil %s@$%d\n", temp_label++, ramec(), unique_num-1);
	max_temp = temp_label; //uchovani nejvyssi dosazene hodnoty

	printf("LABEL $L%d\n", --temp_label);	//format: NIL

	printf("LABEL $L%d\n", --temp_label);	//format: STRING 

	printf("LABEL $L%d\n", --temp_label);	//format: FLOAT

	printf("LABEL $L%d\n", --temp_label);	//format: INT


	temp_label = max_temp;
}


void builtinGenBeginFunc(char *name_of_builtin_function)
{
    printf("JUMP $FOOL%d\n", ++func_cnt);
    printf("LABEL $FNAME%s\n", name_of_builtin_function);
    printf("PUSHFRAME\n");
    //vytvor navratovou promennou
    printf("DEFVAR LF@$RTVL%d\n",func_cnt); //TODO mozna menit ramec
    printf("MOVE LF@$RTVL%d nil@nil\n",func_cnt);//TODO mozna menit ramec   
}

void builtinLength()
{
	builtinGenBeginFunc("length");
	
	printf("STRLEN LF@$RTVL%d LF@$1\n", func_cnt);

	gen_EndFunc();
}

void builtinSubstr()
{
	builtinGenBeginFunc("substr");

	//s - string
	printf("DEFVAR LF@str_s\n");
	printf("MOVE LF@str_s LF%1\n");
	//i - od jake pozice ve stringu
	printf("DEFVAR LF@str_i\n");
	printf("MOVE LF@str_i LF%2\n");
	//n - delka substringu
	printf("DEFVAR LF@str_n\n");
	printf("MOVE LF@str_n LF%3\n");

	//delka stringu
	printf("DEFVAR LF@str_length\n")
	printf("STRLEN LF@str_length LF@str_s\n");

	printf("DEFVAR LF@aux1\n"); // i >= 0
	printf("DEFVAR LF@aux2\n"); // i < len(s)
	//kontrola podminek	
	printf("GT LF@aux1 LF@str_i int@-1\n");
	printf("LT LF@aux2 LF@p2 LF@length\n");
	
	printf("DEFVAR LF@aux3\n"); // i > 0 && i < len(s)
	printf("AND LF@aux3 LF@aux1 LF@aux2\n");
	
	printf("DEFVAR LF@aux4\n"); // n >= 0
	printf("GT LF@aux4 LF@str_n int@-1\n");

	printf("JUMPIFNEQ LEND LF@aux3 LF@aux4\n"); //i > 0 && i < len(s) && n >= 0
	
	/*------------------------------------------------------*/
	/*splneny vsechny podminky --> muzeme vytvorit substring*/
	/*------------------------------------------------------*/	

	printf("DEFVAR LF@cnt\n"); //counter
	printf("MOVE LF@cnt int@0"\n);
	printf("DEFVAR LF@aux5\n"); //kontroluje jestli nejsme na konci stringu
	printf("DEFVAR LF@aux6\n"); //kontroluje jestli jsme uz neulozili n znaku

	printf("LABEL LDO_WHILE\n");

	printf("GETCHAR LF@$RTVL%d LF@str_s LF@str_i\n", func_cnt);
	printf("ADD LF@str_i LF@str_i int@1\n");
	printf("ADD LF@cnt LF@cnt int@1\n");

	//printf("EQ LF@aux5 LF@str_i LF@str_length");
	//printf("NOT LF@aux5 LF@aux5\n");
	printf("JUMPIFEQ LEND LF@str_i LF@str_length\n"); //pokud nejsme na konci retezce tak delej dal
	
	//printf("EQ LF@aux6 LF@cnt LF@str_n"); 	
	//printf("NOT LF@aux6 LF@aux6\n");
	printf("JUMPIF LDO_WHILE LF@cnt LF@str_n\n"); //pokud jsme nezpracovali n znaku tak delej dal


	printf("LABEL LEND\n");	
	gen_EndFunc();
}

void buildinOrd()
{
	builtinGenBeginFunc("ord");

	//s - string
	printf("DEFVAR LF@str_s\n");
	printf("MOVE LF@str_s LF%1\n");
	//i - na jake pozice ve stringu
	printf("DEFVAR LF@str_i\n");
	printf("MOVE LF@str_i LF%2\n");
	
	//delka stringu
	printf("DEFVAR LF@str_length\n")
	printf("STRLEN LF@str_length LF@str_s\n");

	printf("DEFVAR LF@aux1\n");
	printf("DEFVAR LF@aux2\n");

	//kontrola podminek	
	printf("GT LF@aux1 LF@str_i int@-1\n"); // i >= 0
	printf("JUMPIFEQ LEND LF@aux1 bool@false\n");
	printf("LT LF@aux2 LF@p2 LF@length\n"); // i < len(s)
	printf("JUMPIFNEQ LEND LF@aux2 bool@false\n");
		
	/*------------------------------------------------------*/
	/*splneny vsechny podminky --> muzeme vratit ord		*/
	/*------------------------------------------------------*/	

	printf("STRI2INT LF@$RTVL%d LF@str_s LF@str_i\n", func_cnt);
	
	printf("LABEL LEND\n");	
	gen_EndFunc();
}

void buildChr()
{
	builtinGenBeginFunc("ord");
	
	//i - pozice v ascii
	printf("DEFVAR LF@ascii\n");
	printf("MOVE LF@ascii LF%1\n");
	
	//kontrola podminek
	printf("GT LF@aux1 LF@str_i int@-1\n"); // i >=
	printf("JUMPIFNEQ LERROR LF@aux1 bool@true\n");
	printf("LT LF@aux2 LF@p2 int@256\n"); // i =< 255
	printf("JUMPIFNEQ LERROR LF@aux2 bool@true\n");
	
	printf("INT2CHAR LF@$RTVL%d LF@str_i\n", func_cnt);

	printf("JUMP OK")
	//chyba
	printf("LABEL LERROR\n");
	
	printf("#ERROR index error 0-255 ascii\n");
	printf("EXIT int@%d\n", 58); //TODO

	printf("LABEL OK\n");	
	gen_EndFunc();
}
