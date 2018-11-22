/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  					TODO
*/

#include "generator.h"

//potrebne veci pro zacatek definice funkce
void beginFunc()
{
	printf("JUMP %%%d\n", func_cnt);
	printf("LABEL %s\n", "jmeno_funkce");
	printf("PUSHFRAME\n");
	//vytvor navratovou promennou
	printf("DEFVAR LF@%%retval\n");
	printf("MOVE LF@%%retval nil@nil\n");	
	
}

//potrebne veci pro konec definice funkce
void endFunc()
{
	printf("POPFRAME\n");
	printf("RETURN\n");
	printf("LABEL %%%d\n", func_cnt++);
}

//scitani 
void add()
{
	printf("ADD \n");	
}

//definice a inicializace promenne
void defvar(int is_in_func){
	int i; double f;	
	
	if(is_in_func == 0){
		printf("DEFVAR GF@%%%d",uniqueNum);
		switch(*(int*)list->first->data->adr3){
			case T_INT:
				i = strtol((char*)list->first->data->adr2, NULL, 10);
				printf("MOVE GF@%%%d GF@%%%d",uniqueNum,i);
			break;
				f = strtod((char*)list->first->data->adr2, NULL);
				printf("MOVE GF@%%%d GF@%%%f",uniqueNum,f);
			case T_DOUBLE:
			break;
			case T_STRING:
				printf("MOVE GF@%%%d GF@%%%s",uniqueNum,(char*)list->first->data->adr2);
			break;
		}
		
	}
	uniqueNum++;
}

//generovani kodu
int generate()
{
	printf(".IFJcode18\n");
	uniqueNum = 0;
	
	while (list->first != NULL)
	{
		switch(list->first->data->ins_type)
		{
			case PI_BEGINFUNC:
				beginFunc();
				break;
			case PI_ENDFUNC:
				endFunc();
				break;
			case PI_INIT:
				defvar(0);
				break;
		}

		deleteFirst();
	}
	return SUCCESS;
}