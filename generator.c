/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Generator IFJcode18
*/

#include "generator.h"

char* ramec();
void buildChr();
void buildinOrd();
void builtinSubstr();
void builtinLength();
void genBeginFunc();
void genEndFunc();
void genBuiltFunc();
void genFdefPar();
void genFcall();
void genFcallCreateFrame();
void genFcallParT();
void genFcallParID();
void convertTypesAdd();
void convertTypesSub();
void convertTypesMul();
void convertTypesDiv();
void convertTypesIdiv();
void genAddS();
void genAddStr();
void genSubS();
void genMulS();
void genDivS();
void genIdivS();
void genDefVar();
void genAssignDecl();
void genAssign();
void genAssignFunc();
void genAssignDeclFunc();
void convertTypeLtGtLteGte();
void convertTypeEqNeq();
void genIfStart();
void genIfElse();
void genIfEnd();
void genWhileStart();
void genWhileEx();
void genWhileEnd();
void genGte();
void genLte();
void genEq();
void genNeq();
void genDekl();
int generate();


int is_in_func = 0;
int built_func = 1;

//vraci "LF" nebo "GF" podle promene is_in_func
char* ramec(){
	char* a = (is_in_func == 0) ? "GF" : "LF";
	return a;
}

//vestavena funkce chr
void buildChr(){
	//i - pozice v ascii
	printf("DEFVAR LF@ascii\n");
	printf("MOVE LF@ascii LF@$P1\n");

	//kontrola 
	printf("DEFVAR LF@$TYPE%d\n", temp_num);
	
	printf("TYPE LF@$TYPE%d LF@ascii\n", temp_num);	
	printf("JUMPIFNEQ $LABELFLT%d LF@$TYPE%d string@float\n",  temp_label, temp_num);
		printf("FLOAT2INT LF@ascii LF@ascii\n");
		printf("JUMP $LABELFLT2%d\n", temp_label);
	
	printf("LABEL $LABELFLT%d\n", temp_label);
	printf("JUMPIFEQ $LABELFLT2%d LF@$TYPE%d string@int\n", temp_label, temp_num++);
		printf("EXIT int@4\n");
		
	printf("LABEL $LABELFLT2%d\n", temp_label++);
	
	
	printf("DEFVAR LF@aux1\n");
	printf("DEFVAR LF@aux2\n");

	//kontrola podminek
	printf("GT LF@aux1 LF@ascii int@-1\n"); // i >= 0
	printf("JUMPIFNEQ LERROR%d LF@aux1 bool@true\n",func_cnt);
	printf("LT LF@aux2 LF@ascii int@256\n"); // i =< 255
	printf("JUMPIFNEQ LERROR%d LF@aux2 bool@true\n",func_cnt);

	printf("INT2CHAR LF@$RTVL%p LF@ascii\n", (void*)list->first->data->adr1);

	printf("JUMP OK%d\n",func_cnt);
	//chyba
	printf("LABEL LERROR%d\n",func_cnt);

	printf("#ERROR index error 0-255 ascii\n");
	printf("EXIT int@%d\n", 4);

	printf("LABEL OK%d\n",func_cnt);
	printf("PUSHS LF@$RTVL%p\n", (void*)list->first->data->adr1);
}
//vestavena funkce ord
void buildinOrd(){
	//s - string
	printf("DEFVAR LF@str_s\n");
	printf("MOVE LF@str_s LF@$P1\n");
	//i - na jake pozice ve stringu
	printf("DEFVAR LF@str_i\n");
	printf("MOVE LF@str_i LF@$P2\n");

	//kontrola typu
	printf("DEFVAR LF@$TYPE%d\n", temp_num);
	
	printf("TYPE LF@$TYPE%d LF@str_i\n", temp_num);	
	printf("JUMPIFNEQ $LABELFLT%d LF@$TYPE%d string@float\n",  temp_label, temp_num);
		printf("FLOAT2INT LF@str_i LF@str_i\n");
	
	printf("LABEL $LABELFLT%d\n", temp_label);
	
	printf("TYPE LF@$TYPE%d LF@str_s\n", temp_num);	
	printf("JUMPIFNEQ $LABELERR%d LF@$TYPE%d string@string\n",  temp_label, temp_num);
		printf("TYPE LF@$TYPE%d LF@str_i\n", temp_num);
		printf("JUMPIFNEQ $LABELERR%d LF@$TYPE%d string@int\n",  temp_label, temp_num++);
			printf("JUMP $LABELOK%d\n", temp_label);
					
	printf("LABEL $LABELERR%d\n", temp_label);
		printf("EXIT int@4\n");
	
	printf("LABEL $LABELOK%d\n", temp_label++);
	
	//delka stringu
	printf("DEFVAR LF@str_length\n");
	printf("STRLEN LF@str_length LF@str_s\n");

	printf("DEFVAR LF@aux1\n");
	printf("DEFVAR LF@aux2\n");

	//kontrola podminek
	printf("GT LF@aux1 LF@str_i int@-1\n"); // i >= 0
	printf("JUMPIFEQ LEND%d LF@aux1 bool@false\n",func_cnt);
	printf("LT LF@aux2 LF@str_i LF@str_length\n"); // i < len(s)
	printf("JUMPIFEQ LEND%d LF@aux2 bool@false\n",func_cnt);

	/*------------------------------------------------------*/
	/*splneny vsechny podminky --> muzeme vratit ord		*/
	/*------------------------------------------------------*/

	printf("STRI2INT LF@$RTVL%p LF@str_s LF@str_i\n", (void*)list->first->data->adr1);

	printf("LABEL LEND%d\n",func_cnt);
	printf("PUSHS LF@$RTVL%p\n", (void*)list->first->data->adr1);
}
//vestavena funkce substr
void builtinSubstr(){
	//s - string
	printf("DEFVAR LF@str_s\n");
	printf("MOVE LF@str_s LF@$P1\n");
	//i - od jake pozice ve stringu
	printf("DEFVAR LF@str_i\n");
	printf("MOVE LF@str_i LF@$P2\n");
	//n - delka substringu
	printf("DEFVAR LF@str_n\n");
	printf("MOVE LF@str_n LF@$P3\n");

	//kontrola typy
	printf("DEFVAR LF@$TYPE%d\n", temp_num);
	
	printf("TYPE LF@$TYPE%d LF@str_i\n", temp_num);	
	printf("JUMPIFNEQ $LABELFLT%d LF@$TYPE%d string@float\n",  temp_label, temp_num);
		printf("FLOAT2INT LF@str_i LF@str_i\n");
		
	printf("LABEL $LABELFLT%d\n", temp_label);
	printf("TYPE LF@$TYPE%d LF@str_n\n", temp_num);	
	printf("JUMPIFNEQ $LABELFLT2%d LF@$TYPE%d string@float\n",  temp_label, temp_num);
		printf("FLOAT2INT LF@str_n LF@str_n\n");
	
	printf("LABEL $LABELFLT2%d\n", temp_label);
	
	
	printf("TYPE LF@$TYPE%d LF@str_s\n", temp_num);	
	printf("JUMPIFNEQ $LABELERR%d LF@$TYPE%d string@string\n",  temp_label, temp_num);
		printf("TYPE LF@$TYPE%d LF@str_i\n", temp_num);
		printf("JUMPIFNEQ $LABELERR%d LF@$TYPE%d string@int\n",  temp_label, temp_num);
			printf("TYPE LF@$TYPE%d LF@str_n\n", temp_num);
			printf("JUMPIFNEQ $LABELERR%d LF@$TYPE%d string@int\n",  temp_label, temp_num++);
				printf("JUMP $LABELOK%d\n", temp_label);
	
				
	printf("LABEL $LABELERR%d\n", temp_label);
		printf("EXIT int@4\n");
	
	printf("LABEL $LABELOK%d\n", temp_label++);
	
	//jednotlive znaky ktere se budou concatenovat
	printf("DEFVAR LF@tmp_concat\n");

	//delka stringu
	printf("DEFVAR LF@str_length\n");
	printf("STRLEN LF@str_length LF@str_s\n");

	printf("DEFVAR LF@aux1\n"); // i >= 0
	printf("DEFVAR LF@aux2\n"); // i < len(s)
	//kontrola podminek
	printf("GT LF@aux1 LF@str_i int@-1\n");
	printf("LT LF@aux2 LF@str_i LF@str_length\n");

	printf("DEFVAR LF@aux3\n"); // i > 0 && i < len(s)
	printf("AND LF@aux3 LF@aux1 LF@aux2\n");

	printf("DEFVAR LF@aux4\n"); // n >= 0
	printf("GT LF@aux4 LF@str_n int@-1\n");

	printf("JUMPIFNEQ LEND%d LF@aux3 LF@aux4\n",func_cnt); //i > 0 && i < len(s) && n >= 0

	/*------------------------------------------------------*/
	/*splneny vsechny podminky --> muzeme vytvorit substring*/
	/*------------------------------------------------------*/

	printf("DEFVAR LF@cnt\n"); //counter
	printf("MOVE LF@cnt int@0\n");
	printf("DEFVAR LF@aux5\n"); //kontroluje jestli nejsme na konci stringu
	printf("DEFVAR LF@aux6\n"); //kontroluje jestli jsme uz neulozili n znaku

	printf("MOVE LF@$RTVL%p string@\n",(void*)list->first->data->adr1);
	printf("LABEL LDO_WHILE%d\n",func_cnt);


	printf("GETCHAR LF@tmp_concat LF@str_s LF@str_i\n");//maybe
	printf("CONCAT LF@$RTVL%p LF@$RTVL%p LF@tmp_concat\n",(void*)list->first->data->adr1,(void*)list->first->data->adr1);
	printf("ADD LF@str_i LF@str_i int@1\n");
	printf("ADD LF@cnt LF@cnt int@1\n");


	printf("JUMPIFEQ LEND%d LF@str_i LF@str_length\n",func_cnt); //pokud nejsme na konci retezce tak delej dal

	printf("JUMPIFNEQ LDO_WHILE%d LF@cnt LF@str_n\n",func_cnt); //pokud jsme nezpracovali n znaku tak delej dal

	printf("LABEL LEND%d\n",func_cnt);
	printf("PUSHS LF@$RTVL%p\n", (void*)list->first->data->adr1);
}
//vestavena funkce length
void builtinLength(){
	printf("DEFVAR LF@$TYPE%d\n", temp_num);
	printf("TYPE LF@$TYPE%d LF@$P1\n", temp_num);
	
	printf("JUMPIFEQ $LABEL%d LF@$TYPE%d string@string\n",  temp_label, temp_num++);
	printf("EXIT int@4\n");
	
	printf("LABEL $LABEL%d\n", temp_label++);
	printf("STRLEN LF@$RTVL%p LF@$P1\n", (void*)list->first->data->adr1);
	printf("PUSHS LF@$RTVL%p\n", (void*)list->first->data->adr1);
}

//potrebne veci pro zacatek definice funkce
void genBeginFunc(){
	printf("JUMP $FOOL%d\n", ++func_cnt);
	printf("LABEL $FNAME%p\n", list->first->data->adr1);
	printf("PUSHFRAME\n");
	
	//vytvor navratovou promennou
	printf("DEFVAR LF@$RTVL%p\n",list->first->data->adr1); //TODO mozna menit ramec
	printf("MOVE LF@$RTVL%p nil@nil\n",list->first->data->adr1);//TODO mozna menit ramec
	printf("PUSHS LF@$RTVL%p\n",list->first->data->adr1);//TODO mozna menit ramec
}
//potrebne veci pro konec definice funkce
void genEndFunc(){
	printf("POPS LF@$RTVL%p\n",list->first->data->adr1);
	printf("POPFRAME\n");

	printf("RETURN\n");
	printf("LABEL $FOOL%d\n", func_cnt);
}
//vestavene fce na zacatek
void genBuiltFunc(){
	switch(built_func){
		case 1:
			genBeginFunc();
			printf("READ LF@$RTVL%p string\n", (void*)list->first->data->adr1); //inputs - funguje
			printf("PUSHS LF@$RTVL%p\n", (void*)list->first->data->adr1);
			genEndFunc();
			break;
		case 2:
			genBeginFunc();
			printf("READ LF@$RTVL%p int\n", (void*)list->first->data->adr1); //inputi - funguje
			printf("PUSHS LF@$RTVL%p\n", (void*)list->first->data->adr1);
			genEndFunc();
			break;
		case 3:
			genBeginFunc();
			printf("READ LF@$RTVL%p float\n", (void*)list->first->data->adr1); //inputf - funguje
			printf("PUSHS LF@$RTVL%p\n", (void*)list->first->data->adr1);
			genEndFunc();
			break;
		case 4:
			genBeginFunc();
			printf("WRITE LF@$P1\n"); //print - funguje
			printf("PUSHS LF@$RTVL%p\n", (void*)list->first->data->adr1);
			genEndFunc();
			break;
		case 5:
			genBeginFunc();
			builtinLength(); //length - funguje
			genEndFunc();
			break;
		case 6:
			genBeginFunc();
			builtinSubstr(); //substr - funguje
			genEndFunc();
			break;
		case 7:
			genBeginFunc();
			buildinOrd(); //ord - funguje
			genEndFunc();
			break;
		case 8:
			genBeginFunc();
			buildChr(); //chr - funguje
			genEndFunc();
			break;
	}
}
//parametry pri def fuknce vevnitr tela fce
void genFdefPar(){
	printf("DEFVAR LF@$%p\n",(void*)list->first->data->adr1);//tady mozna nemuze byt uniqueNum
	printf("MOVE LF@$%p LF@$P%d\n",(void*)list->first->data->adr1,(void*)list->first->data->adr2);
}
//volani funkce
void genFcall(){
	printf("CALL $FNAME%p\n",list->first->data->adr1);
}
//CREATEFRAME
void genFcallCreateFrame(){
	printf("CREATEFRAME\n");
}
//parametgryT pri volani funkce
void genFcallParT(){
	int i; double f;
	int a = (void*)list->first->data->adr3;
	printf("DEFVAR TF@$P%d\n",list->first->data->adr2);
	switch(a){
		case T_INT:
			i = strtol((char*)list->first->data->adr1, NULL, 10);
			printf("MOVE TF@$P%d int@%d\n",list->first->data->adr2,i);
		break;
		case T_FLOAT:
			f = strtod((void*)list->first->data->adr1, NULL);
			printf("MOVE TF@$P%d float@%a\n",list->first->data->adr2,f);
		break;
		case T_STRING:
			printf("MOVE TF@$P%d string@%s\n",list->first->data->adr2,list->first->data->adr1);
		break;
		case T_NIL:
			printf("MOVE TF@$P%d nil@nil\n",list->first->data->adr2);
		break;
		default:
			printf("WTF %d\n",a);
		break;
	}
	uniqueNum++;
}
//parametgryID pri volani funkce
void genFcallParID(){
	printf("DEFVAR TF@$P%d\n",list->first->data->adr2);
	printf("MOVE TF@$P%d %s@$%p\n",list->first->data->adr2,ramec(),list->first->data->adr1);
}

//konverze typu pro scitani
void convertTypesAdd(){

	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	int aux = temp_label;

	printf("#jsme tu add\n");
	printf("DEFVAR %s@$%d\n", "LF", temp_num++);
	printf("DEFVAR %s@$%d\n", "LF", temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum);

	printf("TYPE %s@$%d %s@$t%d\n", "LF", temp_num-1, "LF", uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", "LF", temp_num, "LF", uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, "LF", temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, "LF", temp_num);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
				//int tmp = uniqueNum;
				genAddStr(); //upravuje hodnotu uniqueNum
				//uniqueNum = tmp;
			printf("JUMP $LEND%d\n", aux); // oba string
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@4\n"); //ERROR prvni string, druhy neni

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, "LF", temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, "LF", temp_num);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
				genAddS();
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					printf("INT2FLOATS\n");
					genAddS();
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@4\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, "LF", temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					genAddS();
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, "LF", temp_num);
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
						genAddS();
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@4\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;

	printf("LABEL $LEND%d\n", aux);

	printf("POPFRAME\n");

}
//konverze typu pro odcitani
void convertTypesSub(){

	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	int aux = temp_label;

	printf("#jsme tu sub\n");
	printf("DEFVAR %s@$%d\n", "LF", temp_num++);
	printf("DEFVAR %s@$%d\n", "LF", temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum);

	//printf("DPRINT %s@$t%d\n", "LF", 0); DEBUG
	//printf("DPRINT %s@$t%d\n", "LF", 1); DEBUG

	printf("TYPE %s@$%d %s@$t%d\n", "LF", temp_num-1, "LF", uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", "LF", temp_num, "LF", uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, "LF", temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, "LF", temp_num);
			printf("EXIT int@4\n"); //ERROR prvni string, druhy neni
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@4\n"); //ERROR prvni string, druhy neni

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, "LF", temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, "LF", temp_num);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
				genSubS();
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					printf("INT2FLOATS\n");
					genSubS();
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@4\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, "LF", temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					genSubS();
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, "LF", temp_num);
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
						genSubS();
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@4\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;
	printf("LABEL $LEND%d\n", aux);


	printf("POPFRAME\n");
}
//konverze typu pro nasobeni
void convertTypesMul(){

	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	int aux = temp_label;

	printf("#jsme tu mul\n");
	printf("DEFVAR %s@$%d\n", "LF", temp_num++);
	printf("DEFVAR %s@$%d\n", "LF", temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum);

	//printf("DPRINT %s@$t%d\n", "LF", 0); DEBUG
	//printf("DPRINT %s@$t%d\n", "LF", 1); DEBUG

	printf("TYPE %s@$%d %s@$t%d\n", "LF", temp_num-1, "LF", uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", "LF", temp_num, "LF", uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, "LF", temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, "LF", temp_num);
			printf("EXIT int@4\n"); //ERROR prvni string, druhy neni
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@4\n"); //ERROR prvni string, druhy neni

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, "LF", temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, "LF", temp_num);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
				genMulS();
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					printf("INT2FLOATS\n");
					genMulS();
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@4\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, "LF", temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					genMulS();
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, "LF", temp_num);
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
						genMulS();
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@4\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;
	printf("LABEL $LEND%d\n", aux);


	printf("POPFRAME\n");
}
//konverze typu pro deleni
void convertTypesDiv(){
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	int aux = temp_label;
	//int tmp, tmpl;

	printf("#jsme tu div\n");
	printf("DEFVAR %s@$%d\n", "LF", temp_num++);
	printf("DEFVAR %s@$%d\n", "LF", temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum);

	printf("TYPE %s@$%d %s@$t%d\n", "LF", temp_num-1, "LF", uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", "LF", temp_num, "LF", uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, "LF", temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, "LF", temp_num);
			printf("EXIT int@4\n"); //ERROR prvni string, druhy neni
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@4\n"); //ERROR prvni string, druhy neni

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, "LF", temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, "LF", temp_num);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);

				genIdivS();

				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					printf("INT2FLOATS\n");

					genDivS();

					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@4\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, "LF", temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);

					genDivS();

					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, "LF", temp_num);
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);

						genDivS();

						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@4\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;
	printf("LABEL $LEND%d\n", aux);
	printf("POPFRAME\n");
}
//konverze typu pro celociselne deleni
void convertTypesIdiv(){
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	int aux = temp_label;
	//int tmp, tmpl;

	printf("#jsme tu idiv\n");
	printf("DEFVAR %s@$%d\n", "LF", temp_num++);
	printf("DEFVAR %s@$%d\n", "LF", temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum);

	printf("TYPE %s@$%d %s@$t%d\n", "LF", temp_num-1, "LF", uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", "LF", temp_num, "LF", uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, "LF", temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, "LF", temp_num);
			printf("EXIT int@4\n"); //ERROR prvni string, druhy neni
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@4\n"); //ERROR prvni string, druhy neni

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, "LF", temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, "LF", temp_num);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);

					genIdivS();

				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					printf("INT2FLOATS\n");

					genIdivS();

					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@4\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, "LF", temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);

					genIdivS();

					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, "LF", temp_num);
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);

						genIdivS();

						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@4\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;
	printf("LABEL $LEND%d\n", aux);
	printf("POPFRAME\n");
}

//scitani
void genAddS(){
	printf("ADDS\n");
}
//scitani (konkatenace) stringu
void genAddStr(){
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	printf("DEFVAR %s@$STR%d\n","LF",only_in_gen);
	printf("POPS %s@$STR%d\n","LF",only_in_gen++);
	printf("DEFVAR %s@$STR%d\n","LF",only_in_gen);
	printf("POPS %s@$STR%d\n","LF",only_in_gen++);
	//vysledek scitani
	printf("DEFVAR %s@$STR%d\n","LF",only_in_gen);
	printf("CONCAT %s@$STR%d %s@$STR%d %s@$STR%d\n","LF",only_in_gen,"LF",only_in_gen-1,"LF",only_in_gen-2);

	printf("PUSHS %s@$STR%d\n","LF",only_in_gen);
	only_in_gen++;
	
	printf("POPFRAME\n");
}
//odcitani
void genSubS(){
	printf("SUBS\n");
}
//nasobeni
void genMulS(){
	printf("MULS\n");
}
//deleni
void genDivS(){
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	printf("DEFVAR %s@$DIV%d\n","LF",only_in_gen);
	printf("POPS %s@$DIV%d\n","LF",only_in_gen++);
	printf("DEFVAR %s@$DIV%d\n","LF",only_in_gen);
	printf("POPS %s@$DIV%d\n","LF",only_in_gen++);
	
	printf("PUSHS %s@$DIV%d\n","LF",only_in_gen - 1);
	printf("PUSHS %s@$DIV%d\n","LF",only_in_gen - 2);

	printf("JUMPIFEQ $DIVL%d float@%a %s@$DIV%d\n", label_in_gen + 1, 0.0, "LF", only_in_gen - 2);

	printf("DIVS\n");
	printf("JUMP $DIVL%d\n", label_in_gen++);

	printf("LABEL $DIVL%d\n",label_in_gen--);

	printf("#ERROR DIVISION BY ZERO \n");

	printf("EXIT int@%d\n",ERROR_ZERO_DIV);

	printf("LABEL $DIVL%d\n",label_in_gen);
	label_in_gen += 2;
	
	printf("POPFRAME\n");
}
//deleni
void genIdivS(){
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	printf("DEFVAR %s@IDIV$%d\n","LF",only_in_gen);
	printf("POPS %s@IDIV$%d\n","LF",only_in_gen++);
	printf("DEFVAR %s@IDIV$%d\n","LF",only_in_gen);
	printf("POPS %s@IDIV$%d\n","LF",only_in_gen++);

	printf("PUSHS %s@IDIV$%d\n","LF",only_in_gen - 1);
	printf("PUSHS %s@IDIV$%d\n","LF",only_in_gen - 2);

	printf("JUMPIFEQ $IDIVL%d int@0 %s@IDIV$%d\n", label_in_gen+1, "LF", only_in_gen - 2); //osetrit ramce

	printf("IDIVS\n");
	printf("JUMP $IDIVL%d\n", label_in_gen++);

	printf("LABEL $IDIVL%d\n",label_in_gen--);

	printf("#ERROR DIVISION BY ZERO \n");

	printf("EXIT int@%d\n",ERROR_ZERO_DIV);

	printf("LABEL $IDIVL%d\n",label_in_gen);
	label_in_gen += 2;
	
	printf("POPFRAME\n");
}
//definice a inicializace promenne
void genDefVar(){

	int i; double f;
		int a = list->first->data->adr3;
		switch(a){
			case T_INT:
				i = strtol(list->first->data->adr2, NULL, 10);
				printf("PUSHS int@%d\n",i);
			break;

			case T_FLOAT:
				f = strtod(list->first->data->adr2, NULL);
				printf("PUSHS float@%a\n",f);
			break;

			case T_STRING:
				printf("PUSHS string@%s\n",list->first->data->adr2);
			break;

			case T_ID:
				printf("PUSHS %s@$%p\n",ramec(),list->first->data->adr2);
			break;

			case T_NIL:
				printf("PUSHS nil@nil\n");
			break;

			default:
				printf("WTF DFT DFTF FŘ%d\n",a);
			break;
		}
	uniqueNum++;
}
//deklarace a prirazeni do idcka
void genAssignDecl(){
	printf("DEFVAR %s@$%p\n",ramec(),list->first->data->adr1);
	printf("POPS %s@$%p\n",ramec(),list->first->data->adr1);
	if(is_in_func == 1){
		printf("PUSHS %s@$%p\n",ramec(),list->first->data->adr1); //kvuli navratove hodnotě fce
	}
}
//prirazeni do idcka
void genAssign(){
	printf("POPS %s@$%p\n",ramec(),list->first->data->adr1);
	if(is_in_func == 1){
		printf("PUSHS %s@$%p\n",ramec(),list->first->data->adr1); //kvuli navratove hodnotě fce
	}
}
//prirazeni do idcka z funkce
void genAssignFunc(){
	printf("MOVE %s@$%p TF@$RTVL%p\n",ramec(),list->first->data->adr1,list->first->data->adr2); //tady potrebuju ukazatel na funkci do symtable
}
//prirazeni do idcka z funkce
void genAssignDeclFunc(){
	printf("DEFVAR %s@$%p\n",ramec(),list->first->data->adr1);
	printf("MOVE %s@$%p TF@$RTVL%p\n",ramec(),list->first->data->adr1,list->first->data->adr2); //tady potrebuju ukazatel na funkci do symtable
}

// > >= < <=
//konverze typu pro relacni operatory
void convertTypeLtGtLteGte(){

	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	int aux = temp_label;

	printf("#jsme tu LtGtLteGte\n");
	printf("DEFVAR %s@$lge%d\n", "LF", temp_num++);
	printf("DEFVAR %s@$lge%d\n", "LF", temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum);

	printf("TYPE %s@$lge%d %s@$t%d\n", "LF", temp_num-1, "LF", uniqueNum-1);
	printf("TYPE %s@$lge%d %s@$t%d\n", "LF", temp_num, "LF", uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$lge%d\n", temp_label++, "LF", temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$lge%d\n", temp_label, "LF", temp_num);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
			printf("JUMP $LEND%d\n", aux); // oba string
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@4\n"); //ERROR prvni string, druhy neni

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$lge%d\n", temp_label++, "LF", temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$lge%d\n", temp_label, "LF", temp_num);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$lge%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					printf("INT2FLOATS\n");
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@4\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$lge%d\n", temp_label++, "LF", temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$lge%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$lge%d\n", temp_label, "LF", temp_num);
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@4\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;
	printf("LABEL $LEND%d\n", aux);


	printf("POPFRAME\n");
}
// == !=
//konverze typu pro rovna se a nerovna se
void convertTypeEqNeq(){

	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	int aux = temp_label;

	printf("#jsme tu EqNeq\n");
	printf("DEFVAR %s@$ene%d\n", "LF", temp_num++);
	printf("DEFVAR %s@$ene%d\n", "LF", temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", "LF", uniqueNum);
	printf("POPS %s@$t%d\n", "LF", uniqueNum);

	printf("TYPE %s@$ene%d %s@$t%d\n", "LF", temp_num-1, "LF", uniqueNum-1);
	printf("TYPE %s@$ene%d %s@$t%d\n", "LF", temp_num, "LF", uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$ene%d\n", temp_label++, "LF", temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$ene%d\n", temp_label, "LF", temp_num);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
			printf("JUMP $LEND%d\n", aux); // oba string
		printf("LABEL $L%d\n", temp_label--);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
				printf("PUSHS nil@nil\n"); //nespravne typy
				printf("JUMP $LEND%d\n", aux);

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$ene%d\n", temp_label++, "LF", temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$ene%d\n", temp_label, "LF", temp_num);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
				//printf("BREAK\n");
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$ene%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					printf("INT2FLOATS\n");
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					printf("PUSHS nil@nil\n"); //nespravne typy
					printf("JUMP $LEND%d\n", aux);

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$ene%d\n", temp_label++, "LF", temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$ene%d\n", temp_label, "LF", temp_num);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
					printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$ene%d\n", temp_label, "LF", temp_num);
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
						printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
						printf("PUSHS nil@nil\n"); //nespravne typy
						printf("JUMP $LEND%d\n", aux);
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum);
				printf("PUSHS %s@$t%d\n", "LF", uniqueNum-1);
				printf("PUSHS nil@nil\n"); //nespravne typy
				printf("JUMP $LEND%d\n", aux);

	temp_num++;
	uniqueNum++;
	printf("LABEL $LEND%d\n", aux);

	printf("POPFRAME\n");
}

//funkce, ktere generuji IF
void genIfStart(){
	int kolikrat_vnoreny = list->first->data->adr1;
	printf("DEFVAR %s@$podminka%d\n",ramec(),podminka_num);
	printf("POPS %s@$podminka%d\n",ramec(),podminka_num);

	//kontrola podminky, pokud neni bool, nil=> false, ostatní věci true
	printf("DEFVAR %s@$Tpodminka%d\n",ramec(),podminka_num);
	printf("TYPE %s@$Tpodminka%d %s@$podminka%d\n",ramec(),podminka_num,ramec(),podminka_num);
	
	printf("JUMPIFEQ $LTESTEND%d %s@$Tpodminka%d string@bool\n",temp_label,ramec(),podminka_num);
	printf("JUMPIFEQ $LTEST%d %s@$Tpodminka%d string@nil\n",temp_label,ramec(),podminka_num);
	
	printf("MOVE %s@$podminka%d bool@true\n",ramec(),podminka_num);
	printf("JUMP $LTESTEND%d\n",temp_label);
	
	printf("LABEL $LTEST%d\n",temp_label);
	
	printf("MOVE %s@$podminka%d bool@false\n",ramec(),podminka_num);
	printf("LABEL $LTESTEND%d\n",temp_label++);
	//konec kontroly
	
	printf("JUMPIFNEQ $IFL%d %s@$podminka%d bool@true\n",if_label + kolikrat_vnoreny ,ramec(),podminka_num++); //todo
	if_max++;

}
void genIfElse(){
	int kolikrat_vnoreny = list->first->data->adr1;
	printf("JUMP $IFEND%d\n",if_label + kolikrat_vnoreny);
	printf("LABEL $IFL%d\n",if_label + kolikrat_vnoreny);
}
void genIfEnd(){
	int kolikrat_vnoreny = list->first->data->adr1;
	printf("LABEL $IFEND%d\n",if_label + kolikrat_vnoreny);
	if (kolikrat_vnoreny == 0){
		if_label += if_max;
		if_max = 0;
	}
}

//funkce, ktere generuji WHILE
void genWhileStart(){
	int kolikrat_vnoreny = list->first->data->adr1;
	printf("DEFVAR %s@$podminka%d\n",ramec(),podminka_num);
	printf("DEFVAR %s@$Tpodminka%d\n",ramec(),podminka_num);
	printf("LABEL $WHILE%d\n",(while_label + kolikrat_vnoreny));
	while_max++;
}
void genWhileEx(){
	
	printf("POPS %s@$podminka%d\n",ramec(),podminka_num);
	
	//kontrola podminky, pokud neni bool, nil=> false, ostatní věci true
	printf("TYPE %s@$Tpodminka%d %s@$podminka%d\n",ramec(),podminka_num,ramec(),podminka_num);
	
	printf("JUMPIFEQ $LTESTEND%d %s@$Tpodminka%d string@bool\n",temp_label,ramec(),podminka_num);
	printf("JUMPIFEQ $LTEST%d %s@$Tpodminka%d string@nil\n",temp_label,ramec(),podminka_num);
	
	printf("MOVE %s@$podminka%d bool@true\n",ramec(),podminka_num);
	printf("JUMP $LTESTEND%d\n",temp_label);
	
	printf("LABEL $LTEST%d\n",temp_label);
	
	printf("MOVE %s@$podminka%d bool@false\n",ramec(),podminka_num);
	printf("LABEL $LTESTEND%d\n",temp_label++);
	//konec kontroly
	
	printf("JUMPIFNEQ $WHILEEND%d %s@$podminka%d bool@true\n",while_label ,ramec(),podminka_num++); //todo
}
void genWhileEnd(){
	int kolikrat_vnoreny = list->first->data->adr1;
	printf("JUMP $WHILE%d\n",while_label + kolikrat_vnoreny);
	printf("LABEL $WHILEEND%d\n",while_label + kolikrat_vnoreny);
	if(is_in_func == 1){
		printf("PUSHS nil@nil\n"); // while vzdycky vraci nil
	}

	if (kolikrat_vnoreny == 0){
		while_label += while_max;
		while_max = 0;
	}
}

//porovnavani - vetsi nez
void genGte(){
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	printf("DEFVAR %s@$GTE%d\n","LF",only_in_gen);
	printf("POPS %s@$GTE%d\n","LF",only_in_gen++);
	printf("DEFVAR %s@$GTE%d\n","LF",only_in_gen);
	printf("POPS %s@$GTE%d\n","LF",only_in_gen++);

	printf("DEFVAR %s@$GTE%d\n","LF",only_in_gen++);
	printf("GT %s@$GTE%d %s@$GTE%d %s@$GTE%d\n","LF",only_in_gen-1,"LF",only_in_gen-2,"LF",only_in_gen-3);
	printf("PUSHS %s@$GTE%d\n","LF",only_in_gen-1);

	printf("DEFVAR %s@$GTE%d\n","LF",only_in_gen++);
	printf("EQ %s@$GTE%d %s@$GTE%d %s@$GTE%d\n","LF",only_in_gen-1,"LF",only_in_gen-3,"LF",only_in_gen-4);
	printf("PUSHS %s@$GTE%d\n","LF",only_in_gen-1);

	printf("ORS\n");

	printf("POPFRAME\n");
}
//porovnavani - mensi nez
void genLte(){
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	printf("DEFVAR %s@$LTE%d\n","LF",only_in_gen);
	printf("POPS %s@$LTE%d\n","LF",only_in_gen++);
	printf("DEFVAR %s@$LTE%d\n","LF",only_in_gen);
	printf("POPS %s@$LTE%d\n","LF",only_in_gen++);

	printf("DEFVAR %s@$LTE%d\n","LF",only_in_gen++);
	printf("LT %s@$LTE%d %s@$LTE%d %s@$LTE%d\n","LF",only_in_gen-1,"LF",only_in_gen-2,"LF",only_in_gen-3);
	printf("PUSHS %s@$LTE%d\n","LF",only_in_gen-1);

	printf("DEFVAR %s@$LTE%d\n","LF",only_in_gen++);
	printf("EQ %s@$LTE%d %s@$LTE%d %s@$LTE%d\n","LF",only_in_gen-1,"LF",only_in_gen-3,"LF",only_in_gen-4);
	printf("PUSHS %s@$LTE%d\n","LF",only_in_gen-1);

	printf("ORS\n");

	printf("POPFRAME\n");
}
//porovnavani - rovna se
void genEq(){
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	printf("DEFVAR %s@$ISNIL%d\n","LF",temp_num);
	printf("POPS %s@$ISNIL%d\n","LF",temp_num);

	printf("DEFVAR %s@$type%d\n","LF",temp_num);
	printf("TYPE %s@$type%d %s@$ISNIL%d\n", "LF",temp_num, "LF",temp_num);
	temp_num++;

	printf("JUMPIFEQ $LISNIL%d %s@$type%d string@nil\n",temp_label,"LF",temp_num-1);

	printf("PUSHS %s@$ISNIL%d\n","LF",temp_num-1);
	printf("EQS\n");
	printf("JUMP $LENDISNIL%d\n",temp_label);

	printf("LABEL $LISNIL%d\n",temp_label++);
	temp_num++;
	
	printf("DEFVAR %s@$ISNIL%d\n","LF",temp_num);
	printf("POPS %s@$ISNIL%d\n","LF",temp_num);
	printf("DEFVAR %s@$type%d\n","LF",temp_num);
	printf("TYPE %s@$type%d %s@$ISNIL%d\n", "LF",temp_num-1, "LF",temp_num++);
	
	printf("DEFVAR %s@$ISNIL%d\n","LF",temp_num);
	printf("POPS %s@$ISNIL%d\n","LF",temp_num);
	printf("DEFVAR %s@$type%d\n","LF",temp_num);
	printf("TYPE %s@$type%d %s@$ISNIL%d\n", "LF",temp_num-1, "LF",temp_num++);
	
	printf("JUMPIFEQ $LONENIL%d %s@$type%d string@nil\n",temp_label,"LF",temp_num-2);//jeden je nil
	printf("JUMP $FALSENIL%d\n",temp_label);
	
	printf("LABEL $LONENIL%d\n",temp_label);
	printf("JUMPIFNEQ $FALSENIL%d %s@$type%d string@nil\n",temp_label,"LF",temp_num-1);//druhy je nil
	printf("PUSHS bool@true\n");//jsou to dva nily
	printf("JUMP $LENDISNIL%d\n",temp_label-1);
	
	printf("LABEL $FALSENIL%d\n",temp_label);
	printf("PUSHS bool@false\n");//nejsou to stejne typy

	printf("LABEL $LENDISNIL%d\n",temp_label-1);
	temp_label++;
	temp_num++;

	printf("POPFRAME\n");

}
//porovnavani - nerovna se
void genNeq(){
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	printf("DEFVAR %s@$ISNIL%d\n","LF",temp_num);
	printf("POPS %s@$ISNIL%d\n","LF",temp_num++);
	printf("DEFVAR %s@$type%d\n","LF",temp_num);
	printf("TYPE %s@$type%d %s@$ISNIL%d\n", "LF",temp_num, "LF",temp_num-1);
	printf("JUMPIFEQ $LISNIL%d %s@$type%d string@nil\n",temp_label,"LF",temp_num);

	printf("PUSHS %s@$ISNIL%d\n","LF",temp_num-1);
	printf("EQS\n");
	printf("NOTS\n");

	printf("JUMP $LENDISNIL%d\n",temp_label);

	printf("LABEL $LISNIL%d\n",temp_label++);

	printf("POPS %s@$ISNIL%d\n","LF",temp_num-1);
	printf("POPS %s@$ISNIL%d\n","LF",temp_num-1);

	printf("PUSHS bool@false\n");

	printf("LABEL $LENDISNIL%d\n",temp_label-1);
	temp_label++;
	temp_num++;
	printf("POPFRAME\n");
}
//porovnavani - deklarace promene bez prirazeni
void genDekl(){
	printf("DEFVAR %s@$%p\n",ramec(),list->first->data->adr1);
	printf("MOVE %s@$%p nil@nil\n",ramec(),list->first->data->adr1);
}

//vypis na merlinovi
// ./main < test2 1> out.ass ; ../rubyset/ic18int ./out.ass

//generovani kodu
int generate()
{
	printf(".IFJcode18\n");

	uniqueNum = 0;
	func_cnt = 0;
	temp_num = 0;
	temp_label = 0;
	only_in_gen = 0;
	while_label = 0;
	podminka_num = 0;
	while_max = 0;
	if_max = 0;
	if_label = 0;

	while (list->first != NULL)
	{
		switch(list->first->data->ins_type)
		{
			case PI_BUILTFUNC:
				genBuiltFunc();
				built_func++;
				break;
			case PI_BEGINFUNC:
				genBeginFunc();
				is_in_func = 1;
				break;
			case PI_DEF_PARAM:
				genFdefPar();
				break;
			case PI_ENDFUNC:
				genEndFunc();
				is_in_func = 0;
				break;
			case INS_CREATEFRAME:
				genFcallCreateFrame();
				break;
			case PI_FCALL:
				genFcall();
				break;
			case PI_FCALL_PARAMT:
				genFcallParT();
				break;
			case PI_FCALL_PARAMID:
				genFcallParID();
				break;
			case PI_INIT:
				genDefVar();
				break;
			case PI_ADD:
				convertTypesAdd();
				break;
			case INS_ADD:
				convertTypesAdd();
				break;
			case INS_SUB:
				convertTypesSub();
				break;
			case INS_MUL:
				convertTypesMul();
				break;
			case INS_DIV:
				convertTypesDiv();
				break;
			case INS_IDIV:
				convertTypesIdiv();
				break;
			case PI_ADDSTR:
				genAddStr();
				break;
			case PI_ASS:
				genAssign();
				break;
			case PI_ASS_DECL:
				genAssignDecl();
				break;
			case PI_ASS_FUNC:
				genAssignFunc();
				break;
			case PI_ASS_DECL_FUNC:
				genAssignDeclFunc();
				break;
			case PI_IF_START:
				genIfStart();
				break;
			case PI_IF_ELSE:
				genIfElse();
				break;
			case PI_IF_END:
				genIfEnd();
				break;
			case PI_WHILE_START:
				genWhileStart();
				break;
			case PI_WHILE_EX:
				genWhileEx();
				break;
			case PI_WHILE_END:
				genWhileEnd();
				break;
			case INS_EQ:
				convertTypeEqNeq();
				genEq();
				break;
			case INS_GT:
				convertTypeLtGtLteGte();
				printf("GTS\n");
				break;
			case INS_LT:
				convertTypeLtGtLteGte();
				printf("LTS\n");
				break;
			case PI_NEQ:
				convertTypeEqNeq();
				genNeq();
				break;
			case PI_GTE:
				convertTypeLtGtLteGte();
				genGte();
				break;
			case PI_LTE:
				convertTypeLtGtLteGte();
				genLte();
				break;
				
			case PI_WHILE_DECL:
				genDekl();
				break;
				
			default:
				break;
		}
		deleteFirst();
	}
	printf("EXIT int@0\n");
	return SUCCESS;
}
