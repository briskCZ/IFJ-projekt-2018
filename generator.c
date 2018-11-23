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
int last_inst = -1;

char* ramec(){
	char* a = (is_in_func == 0) ? "GF" : "LF";
	return a;
}
//potrebne veci pro zacatek definice funkce
void genBeginFunc(){
	printf("JUMP $FOOL%d\n", ++func_cnt);
	printf("LABEL $FNAME%p\n", (void*)list->first->data->adr1);
	printf("PUSHFRAME\n");
	//vytvor navratovou promennou
	printf("DEFVAR LF@$RTVL%d\n",func_cnt); //TODO mozna menit ramec
	printf("MOVE LF@$RTVL%d nil@nil\n",func_cnt);//TODO mozna menit ramec	
	
}
//potrebne veci pro konec definice funkce
void genEndFunc(){
	

	switch (last_inst){
		case PI_ADD:
		case INS_SUB:
		case INS_DIV:
		case INS_IDIV:
			printf("POPS LF@$RTVL%d\n",func_cnt);
		break;
		case PI_ADDSTR:
			printf("MOVE LF@$RTVL%d LF@$%d\n",func_cnt,uniqueNum-1); 
		break;
	}
	
	
	printf("POPFRAME\n");
	printf("RETURN\n");
	printf("LABEL $FOOL%d\n", func_cnt);
	printf("\n");
}

//volani fuckne
void genFcall(){
	printf("#fcall\n");
	printf("CALL $FNAME%p\n",(void*)list->first->data->adr1);
}

void genFcallCreateFrame(){
	printf("CREATEFRAME\n");
}
//parametgryT
void genFcallParT(){
	printf("#genFcallParT \n");
	int i; double f;
	int a = (void*)list->first->data->adr3;
	printf("DEFVAR TF@$%d\n",uniqueNum);
	switch(a){
		case T_INT:
			i = strtol((char*)list->first->data->adr1, NULL, 10);
			printf("MOVE TF@$%d int@%d\n",uniqueNum,i);
		break;
		case T_DOUBLE:
			f = strtod((void*)list->first->data->adr1, NULL);
			printf("MOVE TF@$%d floatg@%a\n",uniqueNum,f);
		break;
		case T_STRING:
			printf("MOVE TF@$%d string@%s\n",uniqueNum,(char*)list->first->data->adr1);
		break;
		
		/*case T_ID:
			printf("PUSHS %s@$%p\n",ramec(),(void*)list->first->data->adr2);				
		break;*/
		
		case T_NIL: //todo
			printf("MOVE %s@$%d %s@$%p\n",ramec(),uniqueNum,ramec(),(void*)list->first->data->adr2);// mozna nefunguje
		break;
		
		default:
			printf("WTF %d\n",a);
		break;
	}
	uniqueNum++;	
}
//parametgryID
void genFcallParID(){
	printf("#genFcallParID \n");
	
	printf("DEFVAR TF@$%d\n",uniqueNum);
	printf("MOVE TF@$%d %s@$%p\n",uniqueNum,ramec(),(void*)list->first->data->adr1);
	uniqueNum++;
}



//konverze typu pro scitani
void convertTypesAdd(unsigned un1, unsigned un2){
	int maxtemp, label;
	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR GF@$t%d\n", temp_num);
	printf("TYPE GF@$t%d GF@$%d\n", temp_num++, un1);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR GF@$t%d\n", temp_num);
	printf("TYPE GF@$t%d GF@$%d\n", temp_num, un2);
	
	label = temp_label++;
	printf("JUMPIFEQ L%d nil@nil GF@$%d\n", temp_label, un1);
	printf("JUMPIFEQ L%d nil@nil GF@$%d\n", temp_label++, un2);
	
	printf("JUMPIFEQ L%d string@string GF@$%d\n", temp_label++, un1);
		printf("JUMPIFEQ L%d GF@$%d GF@$%d\n", temp_label++, un1, un2);
			printf("JUMPIFEQ L%d string@int GF@$%d\n", temp_label++, un2);
				maxtemp = temp_label+1;
				printf("JUMPIFEQ L%d string@float GF@$%d\n", temp_label, un2);
					printf("EXIT int@1\n"); //oba nejsou string
	
	printf("LABEL L%d\n", temp_label--);
	//un1 = INT, un2 = FLOAT
	printf("INT2FLOAT GF@$%d GF@$t%d\n", un1, temp_num-1);
	
	
	printf("LABEL L%d", temp_label--);
	
	
	// uplne na konci
	printf("MOVE GF@$%d GF@", un1, un2);
	
	
	temp_label = maxtemp;
	temp_num++;
}
//scitani 
void genAddS(){
	printf("#add\n");	
	//vysledek scitani na zasobniku
	
	//kontrola typu
	//convertTypesAdd(uniqueNum-3,uniqueNum-2);
	printf("ADDS\n");
	
	printf("\n");	
}
//scitani stringu
void genAddStr(){
	printf("#addStr\n");	
	//vysledek scitani
	printf("DEFVAR %s@$%d\n",ramec(),uniqueNum);//todo ramce
	uniqueNum++;
	//secti
	printf("CONCAT %s@$%d %s@$%d %s@$%d\n",ramec(),uniqueNum-1,ramec(),uniqueNum-3,ramec(),uniqueNum-2);//todo ramce
	printf("\n");	
}
//odcitani
void genSubS(){
	printf("#sub\n");	
	//vysledek scitani
	
	//kontrola typu
	//convertTypesAdd(uniqueNum-3,uniqueNum-2);
	
	printf("SUBS\n");
	
	printf("\n");	
}
//nasobeni
void genMulS(){
	printf("#mul\n");	

	printf("MULS\n");	
	
	printf("\n");	
}
//deleni
void genDivS(){
	printf("#div\n");	
	
	printf("DEFVAR %s@$%d\n",ramec(),uniqueNum);
	printf("POPS %s@$%d\n",ramec(),uniqueNum++);
	printf("DEFVAR %s@$%d\n",ramec(),uniqueNum);
	printf("POPS %s@$%d\n",ramec(),uniqueNum++);
	
	printf("PUSHS %s@$%d\n",ramec(),uniqueNum - 1);
	printf("PUSHS %s@$%d\n",ramec(),uniqueNum - 2);
	
	printf("JUMPIFEQ $L%d float@%a GF@$%d\n", temp_label + 1, 0.0,uniqueNum - 2); //osetrit ramce
	
	printf("DIVS\n");
	printf("JUMP $L%d\n", temp_label++);

	printf("LABEL $L%d\n",temp_label--);
	
	printf("#ERROR DIVISION BY ZERO \n");
	
	printf("EXIT int@%d\n",ERROR_ZERO_DIV);
	
	printf("LABEL $L%d\n",temp_label);
	temp_label += 2;
	
	printf("\n");	
}
//deleni
void genIdivS(){
	printf("#idiv\n");	
	
	printf("DEFVAR %s@$%d\n",ramec(),uniqueNum);
	printf("POPS %s@$%d\n",ramec(),uniqueNum++);
	printf("DEFVAR %s@$%d\n",ramec(),uniqueNum);
	printf("POPS %s@$%d\n",ramec(),uniqueNum++);
	
	printf("PUSHS %s@$%d\n",ramec(),uniqueNum - 1);
	printf("PUSHS %s@$%d\n",ramec(),uniqueNum - 2);
	
	printf("JUMPIFEQ $L%d int@0 GF@$%d\n", temp_label+1, uniqueNum - 2); //osetrit ramce
	
	printf("IDIVS\n");
	printf("JUMP $L%d\n", temp_label++);

	printf("LABEL $L%d\n",temp_label--);
	
	printf("#ERROR DIVISION BY ZERO \n");
	
	printf("EXIT int@%d\n",ERROR_ZERO_DIV);
	
	printf("LABEL $L%d\n",temp_label);
	temp_label += 2;
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
				printf("DEFVAR %s@$%d\n",ramec(),uniqueNum);
				printf("MOVE %s@$%d string@%s\n",ramec(),uniqueNum,(char*)list->first->data->adr2);
			break;
			
			case T_ID:
				printf("PUSHS %s@$%p\n",ramec(),(void*)list->first->data->adr2);				
			break;
			
			case T_NIL:
				printf("MOVE %s@$%d %s@$%p\n",ramec(),uniqueNum,ramec(),(void*)list->first->data->adr2);// mozna nefunguje
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
	
	if(last_inst == PI_ADDSTR){
		printf("MOVE %s@$%p %s@$%d",ramec(),(void*)list->first->data->adr1,ramec(),uniqueNum - 1);
	}else{
		printf("POPS %s@$%p",ramec(),(void*)list->first->data->adr1);
	}
	printf("\n");
}
//prirazeni do idcka
void genAssign(){
	printf("#prirazeni\n");

	if(last_inst == PI_ADDSTR){
		printf("MOVE %s@$%p %s@$%d",ramec(),(void*)list->first->data->adr1,ramec(),uniqueNum - 1);
	}else{
		printf("POPS %s@$%p",ramec(),(void*)list->first->data->adr1);
	}
	
	printf("\n");
}
//prirazeni do idcka z funkce
void genAssignFunc(){//TODO otestovat
	printf("#prirazeni z funkce\n");
	printf("MOVE %s@$%p TF@$RTVL%d\n",ramec(),(void*)list->first->data->adr1,func_cnt);
	printf("\n");
}
//prirazeni do idcka z funkce
void genAssignDeclFunc(){//TODO otestovat
	printf("#prirazeni s deklaraci z funkce\n");
	printf("DEFVAR %s@$%p\n",ramec(),(void*)list->first->data->adr1);
	printf("MOVE %s@$%p TF@$RTVL%d\n",ramec(),(void*)list->first->data->adr1,func_cnt);
	printf("\n");
}

//vypis na merlinovi
// ./main < test2 1> out.ass ; ../rubyset/ic18int ./out.ass

//generovani kodu
int generate()
{
	int test;
	printf(".IFJcode18\n");
	uniqueNum = 0;
	//printList();
	while (list->first != NULL)
	{
		//printf("%d\n", last_inst);
		switch(list->first->data->ins_type)
		{
			case PI_BEGINFUNC:
				genBeginFunc();
				is_in_func = 1;
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
				//ramce
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
				genAddS();
				break;
			case INS_SUB:
				genSubS();
				break;
			case INS_MUL:
				genMulS();
				break;
			case INS_DIV:
				genDivS();
				break;
			case INS_IDIV:
				genIdivS();
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
			default:
				break;
		}
		last_inst = list->first->data->ins_type;
		test = (void*)list->first->data->adr1; //vypis posledniho vysledku prirazeni
		deleteFirst();
	}
	printf("DPRINT GF@$%p\n",test);
	printf("EXIT int@0\n");
	return SUCCESS;
}