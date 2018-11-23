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
int was_move = 0;

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
	printf("POPFRAME\n");
	printf("RETURN\n");
	printf("LABEL $FOOL%d\n", func_cnt);
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
void genAdd(){
	printf("#add\n");	
	//vysledek scitani
	printf("DEFVAR GF@$%d\n",uniqueNum);//todo ramce
	uniqueNum++;
	
	//kontrola typu
	//convertTypesAdd(uniqueNum-3,uniqueNum-2);
	//secti
	
	if (was_move == 1){
		printf("ADD GF@$%d GF@$%d GF@$%d\n",uniqueNum-1,uniqueNum-3,uniqueNum-2);//todo ramce
	}else{
		printf("ADD GF@$%d GF@$%d GF@$%d\n",uniqueNum-1,uniqueNum-5,uniqueNum-2);//todo ramce
	}
	
	was_move = 0;
	
	printf("\n");	
}


//scitani 
void genAddS(){
	printf("#add\n");	
	//vysledek scitani na zasobniku
	//printf("DEFVAR GF@$%d\n",uniqueNum);//todo ramce
	uniqueNum++;
	
	//kontrola typu
	//convertTypesAdd(uniqueNum-3,uniqueNum-2);
	//secti
	
	printf("ADDS\n");
	
	printf("\n");	
}




//scitani stringu
void genAddStr(){
	printf("#addStr\n");	
	//vysledek scitani
	printf("DEFVAR GF@$%d\n",uniqueNum);//todo ramce
	uniqueNum++;
	
	//kontrola typu
	//convertTypesAdd(uniqueNum-3,uniqueNum-2);
	//secti
	printf("CONCAT GF@$%d GF@$%d GF@$%d\n",uniqueNum-1,uniqueNum-3,uniqueNum-2);//todo ramce
	printf("\n");	
}
//odcitani
void genSub(){
	printf("#sub\n");	
	//vysledek scitani
	printf("DEFVAR GF@$%d\n",uniqueNum);//todo ramce
	uniqueNum++;
	
	//kontrola typu
	//convertTypesAdd(uniqueNum-3,uniqueNum-2);
	
	//odecti
	if (was_move == 1){
		printf("SUB GF@$%d GF@$%d GF@$%d\n",uniqueNum-1,uniqueNum-3,uniqueNum-2);//todo ramce
	}else{
		printf("SUB GF@$%d GF@$%d GF@$%d\n",uniqueNum-1,uniqueNum-5,uniqueNum-2);//todo ramce
	}
	
	was_move = 0;
	printf("\n");	
}
//nasobeni
void genMul(){
	printf("#mul\n");	
	//vysledek nasobeni
	printf("DEFVAR GF@$%d\n",uniqueNum);//todo ramce
	uniqueNum++;
	
	if (was_move == 1){
		printf("MUL GF@$%d GF@$%d GF@$%d\n",uniqueNum-1,uniqueNum-3,uniqueNum-2);//todo ramce
	}else{
		printf("MUL GF@$%d GF@$%d GF@$%d\n",uniqueNum-1,uniqueNum-5,uniqueNum-2);//todo ramce
	}

	
	was_move = 0;
	printf("\n");	
}
//definice a inicializace promenne
void genDefVar(){
	
	printf("#defvar\n");	
	int i; double f;
		//printf("DEFVAR %s@$%d\n",ramec(),uniqueNum);//todo ramce
		int a = (void*)list->first->data->adr3;

		switch(a){
			case T_INT:
				i = strtol((char*)list->first->data->adr2, NULL, 10);
				printf("PUSHS int@%d\n",i);
				//printf("MOVE %s@$%d int@%d\n",ramec(),uniqueNum,i);
			break;

			case T_DOUBLE:
				f = strtod((char*)list->first->data->adr2, NULL);
				printf("PUSHS float@%a\n",f);
				//printf("MOVE %s@$%d float@%a\n",ramec(),uniqueNum,f);
			break;
			
			case T_STRING:
				printf("DEFVAR %s@$%d\n",ramec(),uniqueNum);
				printf("MOVE %s@$%d string@%s\n",ramec(),uniqueNum,(char*)list->first->data->adr2);
			break;
			
			case T_ID:
				//printf("MOVE %s@$%d %s@$%p\n",ramec(),uniqueNum,ramec(),(void*)list->first->data->adr2);
				printf("PUSHS %s@$%p\n",i);				
			break;
			
			case T_NIL:
				printf("MOVE %s@$%d %s@$%p\n",ramec(),uniqueNum,ramec(),(void*)list->first->data->adr2);// mozna nefunguje
			break;
			
			default:
				printf("WTF %d\n",a);
			break;
		}
		
	was_move = 1;
	uniqueNum++;
	printf("\n");	
}
//deklarace a prirazeni do idcka
void genAssignDecl(){
	printf("#prirazeni s declaraci\n");
	
	printf("DEFVAR %s@$%p\n",ramec(),(void*)list->first->data->adr1);
	/*
	if ((void*)list->first->data->adr2 != NULL){
		printf("MOVE %s@$%p %s@$%p\n",ramec(),(void*)list->first->data->adr1,ramec(),(void*)list->first->data->adr2);
	}else{
		printf("MOVE %s@$%p %s@$%d\n",ramec(),(void*)list->first->data->adr1,ramec(),uniqueNum-1);
	}*/
	
	printf("POPS %s@$%p",ramec(),(void*)list->first->data->adr1);

	printf("\n");
}
//prirazeni do idcka
void genAssign(){
	printf("#prirazeni\n");

	/*if ((void*)list->first->data->adr2 != NULL){
		printf("MOVE %s@$%p %s@$%p\n",ramec(),(void*)list->first->data->adr1,ramec(),(void*)list->first->data->adr2);
	}else{
		printf("MOVE %s@$%p %s@$%d\n",ramec(),(void*)list->first->data->adr1,ramec(),uniqueNum-1);
	}*/
	
	printf("POPS %s@$%p",ramec(),(void*)list->first->data->adr1);
	
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
		//printf("%d\n",list->first->data->ins_type);
		switch(list->first->data->ins_type)
		{
			case PI_BEGINFUNC:
				genBeginFunc();
				is_in_func = 1;
				break;
			case PI_ENDFUNC:
				genEndFunc();
				is_in_func = 0;
				//genAssignDeclFunc() ;//test
				break;
			case PI_INIT:
				genDefVar();
				break;
			case PI_ADD:
				genAddS();
				break;
			case INS_SUB:
				genSub();
				break;
			case INS_MUL:
				genMul();
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
		
		test = (void*)list->first->data->adr1; //vypis posledniho vysledku prirazeni
		deleteFirst();
	}
	printf("DPRINT GF@$%p\n",test);
	printf("EXIT int@0\n");
	return SUCCESS;
}