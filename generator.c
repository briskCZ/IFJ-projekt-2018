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

int is_in_func = 0;
int built_func = 1;

char* ramec(){
	char* a = (is_in_func == 0) ? "GF" : "LF";
	return a;
}

void buildChr()
{
	//i - pozice v ascii
	printf("DEFVAR LF@ascii\n");
	printf("MOVE LF@ascii LF@$P1\n");
	
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
	printf("EXIT int@%d\n", 58); //TODO

	printf("LABEL OK%d\n",func_cnt);	
	printf("PUSHS LF@$RTVL%p\n", (void*)list->first->data->adr1);
}
void buildinOrd()
{

	//s - string
	printf("DEFVAR LF@str_s\n");
	printf("MOVE LF@str_s LF@$P1\n");
	//i - na jake pozice ve stringu
	printf("DEFVAR LF@str_i\n");
	printf("MOVE LF@str_i LF@$P2\n");
	
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
void builtinSubstr()
{
	//s - string
	printf("DEFVAR LF@str_s\n");
	printf("MOVE LF@str_s LF@$P1\n");
	//i - od jake pozice ve stringu
	printf("DEFVAR LF@str_i\n");
	printf("MOVE LF@str_i LF@$P2\n");
	//n - delka substringu
	printf("DEFVAR LF@str_n\n");
	printf("MOVE LF@str_n LF@$P3\n");
	
	//jednotlive znaky ktere se budou concatenovat
	printf("DEFVAR LF@tmp_concat\n");
	
	//delka stringu
	printf("DEFVAR LF@str_length\n");
	printf("STRLEN LF@str_length LF@str_s\n");

	printf("DEFVAR LF@aux1\n"); // i >= 0
	printf("DEFVAR LF@aux2\n"); // i < len(s)
	//kontrola podminek	
	printf("GT LF@aux1 LF@str_i int@-1\n");
	printf("LT LF@aux2 LF@$P2 LF@str_length\n");
	
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

//potrebne veci pro zacatek definice funkce
void genBeginFunc(){

	printf("JUMP $FOOL%d\n", ++func_cnt);
	printf("LABEL $FNAME%p\n", (void*)list->first->data->adr1);
	printf("PUSHFRAME\n");
	//vytvor navratovou promennou
	printf("DEFVAR LF@$RTVL%p\n",(void*)list->first->data->adr1); //TODO mozna menit ramec
	printf("MOVE LF@$RTVL%p nil@nil\n",(void*)list->first->data->adr1);//TODO mozna menit ramec
	//printf("PUSHS nil@nil\n");
	
}
//potrebne veci pro konec definice funkce
void genEndFunc(){
	printf("POPS LF@$RTVL%p\n",(void*)list->first->data->adr1); //sem potrebuju predat ukazatel do symtable
	printf("POPFRAME\n");
	printf("CLEARS\n");
	printf("RETURN\n");
	printf("LABEL $FOOL%d\n", func_cnt);
	printf("\n");
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
			printf("STRLEN LF@$RTVL%p LF@$P1\n", (void*)list->first->data->adr1); //length - funguje
			printf("PUSHS LF@$RTVL%p\n", (void*)list->first->data->adr1);
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
	printf("#genFdefPar \n");
	printf("DEFVAR LF@$%p\n",(void*)list->first->data->adr1);//tady mozna nemuze byt uniqueNum
	printf("MOVE LF@$%p LF@$P%d\n",(void*)list->first->data->adr1,(void*)list->first->data->adr2);
}
//volani fuckne
void genFcall(){
	printf("#fcall\n");
	printf("CALL $FNAME%p\n",(void*)list->first->data->adr1);
}
//CREATEFRAME
void genFcallCreateFrame(){
	printf("CREATEFRAME\n");
}
//parametgryT 
void genFcallParT(){
	printf("#genFcallParT \n");
	int i; double f;
	int a = (void*)list->first->data->adr3;
	printf("DEFVAR TF@$P%d\n",(void*)list->first->data->adr2);
	switch(a){
		case T_INT:
			i = strtol((char*)list->first->data->adr1, NULL, 10);
			printf("MOVE TF@$P%d int@%d\n",(void*)list->first->data->adr2,i);
		break;
		case T_DOUBLE:
			f = strtod((void*)list->first->data->adr1, NULL);
			printf("MOVE TF@$P%d float@%a\n",(void*)list->first->data->adr2,f);
		break;
		case T_STRING:
			printf("MOVE TF@$P%d string@%s\n",(void*)list->first->data->adr2,(char*)list->first->data->adr1);
		break;		
		case T_NIL: //todo
			printf("MOVE TF@$P%d nil@nil\n",(void*)list->first->data->adr2);
		break;
		default:
			printf("WTF %d\n",a);
		break;
	}
	uniqueNum++;	
}
//parametgryID pri volani funkce
void genFcallParID(){
	printf("#genFcallParID \n");
	
	printf("DEFVAR TF@$P%d\n",(void*)list->first->data->adr2);
	printf("MOVE TF@$P%d %s@$%p\n",(void*)list->first->data->adr2,ramec(),(void*)list->first->data->adr1);
}

//konverze typu pro scitani
void convertTypesAdd(){
	int aux = temp_label;

	printf("#jsme tu\n");
	printf("DEFVAR %s@$%d\n", ramec(), temp_num++);
	printf("DEFVAR %s@$%d\n", ramec(), temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum);

	//printf("DPRINT %s@$t%d\n", ramec(), 0); DEBUG
	//printf("DPRINT %s@$t%d\n", ramec(), 1); DEBUG

	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num-1, ramec(), uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num, ramec(), uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, ramec(), temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
				//int tmp = uniqueNum;
				genAddStr(); //upravuje hodnotu uniqueNum
				//uniqueNum = tmp;
			printf("JUMP $LEND%d\n", aux); // oba string
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@1\n"); //ERROR prvni string, druhy neni

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, ramec(), temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
				genAddS();
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					printf("INT2FLOATS\n");
					genAddS();
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@2\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, ramec(), temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					genAddS();
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
						genAddS();
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@3\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;
printf("LABEL $LEND%d\n", aux);
}

void convertTypesSub(){
	int aux = temp_label;

	printf("#jsme tu\n");
	printf("DEFVAR %s@$%d\n", ramec(), temp_num++);
	printf("DEFVAR %s@$%d\n", ramec(), temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum);

	//printf("DPRINT %s@$t%d\n", ramec(), 0); DEBUG
	//printf("DPRINT %s@$t%d\n", ramec(), 1); DEBUG

	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num-1, ramec(), uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num, ramec(), uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, ramec(), temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, ramec(), temp_num);
			printf("EXIT int@1\n"); //ERROR prvni string, druhy neni		
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@1\n"); //ERROR prvni string, druhy neni	

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, ramec(), temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
				genSubS();
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					printf("INT2FLOATS\n");
					genSubS();
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@2\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, ramec(), temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					genSubS();
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
						genSubS();
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@3\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;
printf("LABEL $LEND%d\n", aux);
}

void convertTypesMul(){
	int aux = temp_label;

	printf("#jsme tu\n");
	printf("DEFVAR %s@$%d\n", ramec(), temp_num++);
	printf("DEFVAR %s@$%d\n", ramec(), temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum);

	//printf("DPRINT %s@$t%d\n", ramec(), 0); DEBUG
	//printf("DPRINT %s@$t%d\n", ramec(), 1); DEBUG

	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num-1, ramec(), uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num, ramec(), uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, ramec(), temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, ramec(), temp_num);
			printf("EXIT int@1\n"); //ERROR prvni string, druhy neni		
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@1\n"); //ERROR prvni string, druhy neni	

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, ramec(), temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
				genMulS();
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					printf("INT2FLOATS\n");
					genMulS();
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@2\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, ramec(), temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					genMulS();
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
						genMulS();
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@3\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;
printf("LABEL $LEND%d\n", aux);
}

void convertTypesDiv(){
	int aux = temp_label;
	//int tmp, tmpl;

	printf("#jsme tu\n");
	printf("DEFVAR %s@$%d\n", ramec(), temp_num++);
	printf("DEFVAR %s@$%d\n", ramec(), temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum);

	//printf("DPRINT %s@$t%d\n", ramec(), 0); DEBUG
	//printf("DPRINT %s@$t%d\n", ramec(), 1); DEBUG

	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num-1, ramec(), uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num, ramec(), uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, ramec(), temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, ramec(), temp_num);
			printf("EXIT int@1\n"); //ERROR prvni string, druhy neni		
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@1\n"); //ERROR prvni string, druhy neni	

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, ramec(), temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
				
				//tmp = uniqueNum;
				//tmpl = temp_label;
				genDivS();
				//uniqueNum = tmp;
				//temp_label = tmpl;
				
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					printf("INT2FLOATS\n");
					
					//tmp = uniqueNum;
					//tmpl = temp_label;
					genDivS();
					//uniqueNum = tmp;
					//temp_label = tmpl;
					
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@2\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, ramec(), temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					
					//tmp = uniqueNum;
					//tmpl = temp_label;
					genDivS();
					//uniqueNum = tmp;
					//temp_label = tmpl;
					
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
						
						//tmp = uniqueNum;
						//tmpl = temp_label;
						genDivS();
						//uniqueNum = tmp;
						//temp_label = tmpl;
						
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@3\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;
printf("LABEL $LEND%d\n", aux);
}

void convertTypesIdiv(){
	int aux = temp_label;
	//int tmp, tmpl;

	printf("#jsme tu\n");
	printf("DEFVAR %s@$%d\n", ramec(), temp_num++);
	printf("DEFVAR %s@$%d\n", ramec(), temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum);

	//printf("DPRINT %s@$t%d\n", ramec(), 0); DEBUG
	//printf("DPRINT %s@$t%d\n", ramec(), 1); DEBUG

	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num-1, ramec(), uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num, ramec(), uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, ramec(), temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, ramec(), temp_num);
			printf("EXIT int@1\n"); //ERROR prvni string, druhy neni		
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@1\n"); //ERROR prvni string, druhy neni	

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, ramec(), temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					
					//tmp = uniqueNum;
					//tmpl = temp_label;
					genIdivS();
					//uniqueNum = tmp;
					//temp_label = tmpl;
					
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					printf("INT2FLOATS\n");
					
					//tmp = uniqueNum;
					//tmpl = temp_label;
					genIdivS();
					//uniqueNum = tmp;
					//temp_label = tmpl;
					
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@2\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, ramec(), temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					
					//tmp = uniqueNum;
					//tmpl = temp_label;
					genIdivS();
					//uniqueNum = tmp;
					//temp_label = tmpl;
					
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					
						//tmp = uniqueNum;
						//tmpl = temp_label;
						genIdivS();
						//uniqueNum = tmp;
						//temp_label = tmpl;
					
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@3\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;
printf("LABEL $LEND%d\n", aux);
}


//scitani 
void genAddS(){
	printf("#add\n");	
	//kontrola typu
	//convertTypesAdd(uniqueNum-3,uniqueNum-2);
	printf("ADDS\n");
	printf("\n");	
}
//scitani stringu
void genAddStr(){
	printf("#addStr\n");	

	printf("DEFVAR %s@$STR%d\n",ramec(),only_in_gen);
	printf("POPS %s@$STR%d\n",ramec(),only_in_gen++);
	printf("DEFVAR %s@$STR%d\n",ramec(),only_in_gen);
	printf("POPS %s@$STR%d\n",ramec(),only_in_gen++);
	//vysledek scitani
	printf("DEFVAR %s@$STR%d\n",ramec(),only_in_gen);
	printf("CONCAT %s@$STR%d %s@$STR%d %s@$STR%d\n",ramec(),only_in_gen,ramec(),only_in_gen-1,ramec(),only_in_gen-2);
	
	printf("PUSHS %s@$STR%d\n",ramec(),only_in_gen);
	only_in_gen++;
	
	printf("\n");	
}
//odcitani
void genSubS(){
	printf("#sub\n");	
	//kontrola typu
	printf("SUBS\n");
	printf("\n");	
}
//nasobeni
void genMulS(){
	printf("#mul\n");	
	//kontrola typu
	printf("MULS\n");	
	printf("\n");	
}
//deleni
void genDivS(){
	printf("#div\n");	
	
	printf("DEFVAR %s@$DIV%d\n",ramec(),only_in_gen);
	printf("POPS %s@$DIV%d\n",ramec(),only_in_gen++);
	printf("DEFVAR %s@$DIV%d\n",ramec(),only_in_gen);
	printf("POPS %s@$DIV%d\n",ramec(),only_in_gen++);
	
	printf("PUSHS %s@$DIV%d\n",ramec(),only_in_gen - 1);
	printf("PUSHS %s@$DIV%d\n",ramec(),only_in_gen - 2);
	
	printf("JUMPIFEQ $DIVL%d float@%a %s@$DIV%d\n", label_in_gen + 1, 0.0, ramec(), only_in_gen - 2); //osetrit ramce
	
	printf("DIVS\n");
	printf("JUMP $DIVL%d\n", label_in_gen++);

	printf("LABEL $DIVL%d\n",label_in_gen--);
	
	printf("#ERROR DIVISION BY ZERO \n");
	
	printf("EXIT int@%d\n",ERROR_ZERO_DIV);
	
	printf("LABEL $DIVL%d\n",label_in_gen);
	label_in_gen += 2;
	
	printf("\n");	
}
//deleni
void genIdivS(){
	printf("#idiv\n");	
	
	printf("DEFVAR %s@IDIV$%d\n",ramec(),only_in_gen);
	printf("POPS %s@IDIV$%d\n",ramec(),only_in_gen++);
	printf("DEFVAR %s@IDIV$%d\n",ramec(),only_in_gen);
	printf("POPS %s@IDIV$%d\n",ramec(),only_in_gen++);
	
	printf("PUSHS %s@IDIV$%d\n",ramec(),only_in_gen - 1);
	printf("PUSHS %s@IDIV$%d\n",ramec(),only_in_gen - 2);
	
	printf("JUMPIFEQ $IDIVL%d int@0 %s@IDIV$%d\n", label_in_gen+1, ramec(), only_in_gen - 2); //osetrit ramce
	
	printf("IDIVS\n");
	printf("JUMP $IDIVL%d\n", label_in_gen++);

	printf("LABEL $IDIVL%d\n",label_in_gen--);
	
	printf("#ERROR DIVISION BY ZERO \n");
	
	printf("EXIT int@%d\n",ERROR_ZERO_DIV);
	
	printf("LABEL $IDIVL%d\n",label_in_gen);
	label_in_gen += 2;
	printf("\n");	
}
//definice a inicializace promenne
void genDefVar(){
	
	printf("#defvar\n");	
	int i; double f;
		int a = (void*)list->first->data->adr3;

		switch(a){
			case T_INT:
				i = strtol((char*)list->first->data->adr2, NULL, 10);
				printf("PUSHS int@%d\n",i);
			break;

			case T_DOUBLE:
				f = strtod((void*)list->first->data->adr2, NULL);
				printf("PUSHS float@%a\n",f);
			break;
			
			case T_STRING:
				printf("PUSHS string@%s\n",(void*)list->first->data->adr2);
			break;
			
			case T_ID:
				printf("PUSHS %s@$%p\n",ramec(),(void*)list->first->data->adr2);				
			break;
			
			case T_NIL:
				printf("PUSHS nil@nil\n");
			break;
			
			default:
				printf("WTF %d\n",a);
			break;
		}
		
	uniqueNum++;
	printf("\n");	
}
//deklarace a prirazeni do idcka
void genAssignDecl(){
	printf("#prirazeni s declaraci\n");
	printf("DEFVAR %s@$%p\n",ramec(),(void*)list->first->data->adr1);
	printf("POPS %s@$%p",ramec(),(void*)list->first->data->adr1);
	printf("\n");
}
//prirazeni do idcka
void genAssign(){
	printf("#prirazeni\n");
	printf("POPS %s@$%p",ramec(),(void*)list->first->data->adr1);
	printf("\n");
}
//prirazeni do idcka z funkce
void genAssignFunc(){//TODO otestovat
	printf("#prirazeni z funkce\n");
	printf("MOVE %s@$%p TF@$RTVL%p\n",ramec(),(void*)list->first->data->adr1,(void*)list->first->data->adr2); //tady potrebuju ukazatel na funkci do symtable
	printf("\n");
}
//prirazeni do idcka z funkce
void genAssignDeclFunc(){//TODO otestovat
	printf("#prirazeni s deklaraci z funkce\n");
	printf("DEFVAR %s@$%p\n",ramec(),(void*)list->first->data->adr1);
	printf("MOVE %s@$%p TF@$RTVL%p\n",ramec(),(void*)list->first->data->adr1,(void*)list->first->data->adr2); //tady potrebuju ukazatel na funkci do symtable
	printf("\n");
}

// > >= < <=
void convertTypeLtGtLteGte()
{
	int aux = temp_label;

	printf("#jsme tu\n");
	printf("DEFVAR %s@$%d\n", ramec(), temp_num++);
	printf("DEFVAR %s@$%d\n", ramec(), temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum);

	//printf("DPRINT %s@$t%d\n", ramec(), 0); DEBUG
	//printf("DPRINT %s@$t%d\n", ramec(), 1); DEBUG

	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num-1, ramec(), uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num, ramec(), uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, ramec(), temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
				//int tmp = uniqueNum;
				//genAddStr(); //upravuje hodnotu uniqueNum
				//uniqueNum = tmp;
			printf("JUMP $LEND%d\n", aux); // oba string
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@1\n"); //ERROR prvni string, druhy neni

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, ramec(), temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
				//genAddS();
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					printf("INT2FLOATS\n");
					//genAddS();
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@2\n"); //ERROR prvni int | druhy string

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, ramec(), temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					//genAddS();
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
						genAddS();
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@3\n"); //ERROR prvni float | druhy string
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@4\n"); //ERROR oba nejsou int nebo float

	temp_num++;
	uniqueNum++;
	printf("LABEL $LEND%d\n", aux);
}

// == !=
void convertTypeEqNeq()
{
	int aux = temp_label;

	printf("#jsme tu\n");
	printf("DEFVAR %s@$%d\n", ramec(), temp_num++);
	printf("DEFVAR %s@$%d\n", ramec(), temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum++);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", ramec(), uniqueNum);
	printf("POPS %s@$t%d\n", ramec(), uniqueNum);

	//printf("DPRINT %s@$t%d\n", ramec(), 0); DEBUG
	//printf("DPRINT %s@$t%d\n", ramec(), 1); DEBUG

	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num-1, ramec(), uniqueNum-1);
	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num, ramec(), uniqueNum);

	/* 				kontrola typu */
	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, ramec(), temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
				//int tmp = uniqueNum;
				//genAddStr(); //upravuje hodnotu uniqueNum
				//uniqueNum = tmp;
			printf("JUMP $LEND%d\n", aux); // oba string
		printf("LABEL $L%d\n", temp_label--);
			printf("PUSHS nil@nil"); //nespravne typy

	printf("LABEL $L%d\n", temp_label); //prvni neni string
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, ramec(), temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
				printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
				//genAddS();
				printf("JUMP $LEND%d\n", aux); //oba int
			printf("LABEL $L%d\n", temp_label++); //prvni int | druhy neni int
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					printf("INT2FLOATS\n");
					//genAddS();
					printf("JUMP $LEND%d\n", aux); //prvni int | druhy float
				printf("LABEL $L%d\n", temp_label);
					printf("PUSHS nil@nil"); //nespravne typy

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label); //prvni je float
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, ramec(), temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
					printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
					//genAddS();
					printf("JUMP $LEND%d\n", aux); //prvni float |  druhy float
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum);
						printf("INT2FLOATS\n");
						printf("PUSHS %s@$t%d\n", ramec(), uniqueNum-1);
						genAddS();
						printf("JUMP $LEND%d\n", aux); //prvni float | druhy int
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("PUSHS nil@nil"); //nespravne typy
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("PUSHS nil@nil"); //nespravne typy

	temp_num++;
	uniqueNum++;
	printf("LABEL $LEND%d\n", aux);
}

void genIfStart(){
	
	printf("DEFVAR %s@$podminka%d\n",ramec(),uniqueNum);
	printf("POPS %s@$podminka%d\n",ramec(),uniqueNum);

	printf("#if start\n");	

	printf("JUMPIFNEQ $IFL%d %s@$podminka%d bool@true\n",temp_label,ramec(),uniqueNum++); //todo	
	
	printf("\n");	
}
void genIfElse(){
	printf("#if else\n");	
	printf("JUMP $IFEND%d\n",++temp_label);
	printf("LABEL $IFL%d\n",temp_label-1);	
	
	printf("\n");	
}
void genIfEnd(){
	printf("LABEL $IFEND%d\n",temp_label++);
}

void genWhileStart(){
	printf("#while start\n");
	printf("DEFVAR %s@$podminka%d\n",ramec(),temp_num);
	printf("LABEL $WHILE%d\n",temp_label++);
}
void genWhileEx(){
	printf("#while ex\n");
	printf("POPS %s@$podminka%d\n",ramec(),temp_num);
	printf("JUMPIFNEQ $WHILEEND%d %s@$podminka%d bool@true\n",temp_label,ramec(),temp_num++); //todo
}
void genWhileEnd(){
	printf("#while end\n");
	printf("JUMP $WHILE%d\n",temp_label-1);
	printf("LABEL $WHILEEND%d\n",temp_label++);
}

void genGte(){
	printf("#Gte\n");
	
	printf("DEFVAR %s@$GTE%d\n",ramec(),only_in_gen);
	printf("POPS %s@$GTE%d\n",ramec(),only_in_gen++);
	printf("DEFVAR %s@$GTE%d\n",ramec(),only_in_gen);
	printf("POPS %s@$GTE%d\n",ramec(),only_in_gen++);
	
	printf("DEFVAR %s@$GTE%d\n",ramec(),only_in_gen++);
	printf("GT %s@$GTE%d %s@$GTE%d %s@$GTE%d\n",ramec(),only_in_gen-1,ramec(),only_in_gen-2,ramec(),only_in_gen-3);	
	printf("PUSHS %s@$GTE%d\n",ramec(),only_in_gen-1);
	
	printf("DEFVAR %s@$GTE%d\n",ramec(),only_in_gen++);
	printf("EQ %s@$GTE%d %s@$GTE%d %s@$GTE%d\n",ramec(),only_in_gen-1,ramec(),only_in_gen-3,ramec(),only_in_gen-4);	
	printf("PUSHS %s@$GTE%d\n",ramec(),only_in_gen-1);
	
	printf("ORS\n");
}

void genLte(){
	printf("#Lte\n");

	printf("DEFVAR %s@$LTE%d\n",ramec(),only_in_gen);
	printf("POPS %s@$LTE%d\n",ramec(),only_in_gen++);
	printf("DEFVAR %s@$LTE%d\n",ramec(),only_in_gen);
	printf("POPS %s@$LTE%d\n",ramec(),only_in_gen++);
	
	printf("DEFVAR %s@$LTE%d\n",ramec(),only_in_gen++);
	printf("LT %s@$LTE%d %s@$LTE%d %s@$LTE%d\n",ramec(),only_in_gen-1,ramec(),only_in_gen-2,ramec(),only_in_gen-3);	
	printf("PUSHS %s@$LTE%d\n",ramec(),only_in_gen-1);
	
	printf("DEFVAR %s@$LTE%d\n",ramec(),only_in_gen++);
	printf("EQ %s@$LTE%d %s@$LTE%d %s@$LTE%d\n",ramec(),only_in_gen-1,ramec(),only_in_gen-3,ramec(),only_in_gen-4);	
	printf("PUSHS %s@$LTE%d\n",ramec(),only_in_gen-1);
	
	printf("ORS\n");	
	
	
	
	
}





//vypis na merlinovi
// ./main < test2 1> out.ass ; ../rubyset/ic18int ./out.ass

//generovani kodu
int generate()
{
	int test;
	printf(".IFJcode18\n");
	uniqueNum = 0;
	func_cnt = 0; //pocitadlo funkci
	temp_num = 0;
	temp_label = 0;
	only_in_gen = 0;
	//printList();
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
				printf("EQS\n");
				break;
			case INS_GT:
				printf("GTS\n");
				break;
			case INS_LT:
				printf("LTS\n");
				break;
			case PI_NEQ:
				printf("EQS\n");
				printf("NOTS\n");
				break;
			case PI_GTE:
				genGte();
				break;
			case PI_LTE:
				genLte();
				break;			

				
			default:
				break;
		}
		test = (void*)list->first->data->adr1; //vypis posledniho vysledku prirazeni
		deleteFirst();
	}
	printf("EXIT int@0\n");
	return SUCCESS;
}