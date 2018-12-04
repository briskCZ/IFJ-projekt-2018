/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  implementace funkci pracujici nad ADT dvojsmerne vazany seznamem
*/

#include "ins_list.h"

// vytvori list pro instrukce
int listInit(){

	list = malloc(sizeof(t_InsList));
	if (list == NULL)
		return MEMORY_ERROR;

	list->first = NULL;
	list->act = NULL;
	list->last = NULL;
	return MEMORY_OK;
}

// nastavi aktivni prvek
void setActive(t_Elem *elem)
{
	list->act = elem;
}

//prida instrukci
int addInst(int ins_type, void *adr1, void *adr2, void *adr3, int act)
{
	t_Ins *new = malloc(sizeof(t_Ins));
	if (new == NULL)
		return MEMORY_ERROR;

	new->ins_type = ins_type;
	new->adr1 = adr1;
	new->adr2 = adr2;
	new->adr3 = adr3;

	if (act == 0)
		insertLast(new);
	else
		insertBeforeActive(new);
	return MEMORY_OK;
}

//vlozi na posledni prvek seznamu
int insertLast(t_Ins *ins)
{
	if (list == NULL)
	{
		fprintf(stderr, "ERROR: inslist neexistuje\n");
		exit(ERROR_INTERNAL);
	}

	t_Elem *new = malloc(sizeof(t_Elem));
	if (new == NULL)
		return MEMORY_ERROR;

	new->data = ins;
	new->next = NULL;
	new->prev = list->last;

	if (list->first == NULL || list->last == NULL)
	{
		list->first = list->last = new;
	}
	else //pokud neni list prazdny
	{
		list->last->next = new;
		list->last = new;
	}

	return MEMORY_OK;
}

//vlozi pred aktivni prvek
int insertBeforeActive(t_Ins *ins)
{
	if (list->act == NULL)
	{
		fprintf(stderr, "ERROR: list neni aktivni\n");
		exit(ERROR_INTERNAL);
	}

	t_Elem *new = malloc(sizeof(t_Elem));
	if (new == NULL)
		return MEMORY_ERROR;

	new->data = ins;
	new->next = list->act;
	new->prev = list->act->prev;

	if (list->act->prev != NULL)
	{
		list->act->prev->next = new;
	}
	else
	{
		list->first = new;
	}
	list->act->prev = new;

	return MEMORY_OK;
}

//prepise aktivni prvek
void rewriteActive(int ins_type, void *adr1, void *adr2, void *adr3)
{
	if (list->act == NULL)
	{
		fprintf(stderr, "ERROR: list neni aktivni\n");
		exit(ERROR_INTERNAL);
	}

	list->act->data->ins_type = ins_type;
	list->act->data->adr1 = adr1;
	list->act->data->adr2 = adr2;
	list->act->data->adr3 = adr3;
}

//samze prvni prvek
void deleteFirst()
{
	if (list->first == list->act)
	{
		list->act = NULL;
	}
	t_Elem *temp = list->first;
	list->first->prev = NULL;
	list->first = list->first->next;

	free(temp->data);
	free(temp);
}

//uvolni list
void freeList()
{
	while(list->first != NULL)
	{
		t_Elem *temp = list->first;
		list->first = list->first->next;
		free(temp->data);
		free(temp);
		//fprintf(stderr, "a\n");
	}
	list->act = list->last = NULL;
	free(list);
}

/*----------------------*/
/*	debugovaci funkce	*/
/*----------------------*/

//vypise list
void printList()
{
	t_Elem *temp = list->first;

	while (temp != NULL)
	{
		fprintf(stderr, "Act: %d | ID: %d: ", (temp == list->act) ? 1 : 0, temp->data->ins_type);
		/*if (temp->data->adr1 != NULL){
			t_Node *n = temp->data->adr1;
			if (n != NULL){
				if (n > 1000){
					fprintf(stderr, " VAL: %s | ", n->data->name->val);
				}
			}
		}*/
		switch (temp->data->ins_type){
			case INS_MOVE: fprintf(stderr, "INS_MOVE\n"); break;
			case INS_CREATEFRAME: fprintf(stderr, "INS_CREATEFRAME\n"); break;
			case INS_PUSHFRAME: fprintf(stderr, "INS_PUSHFRAME\n"); break;
			case INS_POPFRAME: fprintf(stderr, "INS_POPFRAME\n"); break;
			case INS_DEFVAR: fprintf(stderr, "INS_DEFVAR\n"); break;
			case INS_CALL: fprintf(stderr, "INS_CALL\n"); break;
			case INS_RETURN: fprintf(stderr, "INS_RETURN\n"); break;

			case INS_PUSHS: fprintf(stderr, "INS_PUSHS\n"); break;
			case INS_POPS: fprintf(stderr, "INS_POPS\n"); break;
			case INS_CLEARS: fprintf(stderr, "INS_CLEARS\n"); break;

			case INS_ADD: fprintf(stderr, "INS_ADD\n"); break;
			case INS_SUB: fprintf(stderr, "INS_SUB\n"); break;
			case INS_MUL: fprintf(stderr, "INS_MUL\n"); break;
			case INS_DIV: fprintf(stderr, "INS_DIV\n"); break;
			case INS_IDIV: fprintf(stderr, "INS_IDIV\n"); break;

			case INS_ADDS: fprintf(stderr, "INS_ADDS\n"); break;
			case INS_SUBS: fprintf(stderr, "INS_SUBS\n"); break;
			case INS_MULS: fprintf(stderr, "INS_MULS\n"); break;
			case INS_DIVS: fprintf(stderr, "INS_DIVS\n"); break;
			case INS_IDIVS: fprintf(stderr, "INS_IDIVS\n"); break;

			case INS_LT: fprintf(stderr, "INS_LT\n"); break;
			case INS_GT: fprintf(stderr, "INS_GT\n"); break;
			case INS_EQ: fprintf(stderr, "INS_EQ\n"); break;

			case INS_LTS: fprintf(stderr, "INS_LTS\n"); break;
			case INS_GTS: fprintf(stderr, "INS_GTS\n"); break;
			case INS_EQS: fprintf(stderr, "INS_EQS\n"); break;

			case INS_AND: fprintf(stderr, "INS_AND\n"); break;
			case INS_OR: fprintf(stderr, "INS_OR\n"); break;
			case INS_NOT: fprintf(stderr, "INS_NOT\n"); break;

			case INS_ANDS: fprintf(stderr, "INS_ANDS\n"); break;
			case INS_ORS: fprintf(stderr, "INS_ORS\n"); break;
			case INS_NOTS: fprintf(stderr, "INS_NOTS\n"); break;

			case INS_INT2FLOAT: fprintf(stderr, "INS_INT2FLOAT\n"); break;
			case INS_FLOAT2INT: fprintf(stderr, "INS_FLOAT2INT\n"); break;
			case INS_INT2CHAR: fprintf(stderr, "INS_INT2CHAR\n"); break;
			case INS_STRI2INT: fprintf(stderr, "INS_STRI2INT\n"); break;

			case INS_INT2FLOATS: fprintf(stderr, "INS_INT2FLOATS\n"); break;
			case INS_FLOAT2INTS: fprintf(stderr, "INS_FLOAT2INTS\n"); break;
			case INS_INT2CHARS: fprintf(stderr, "INS_INT2CHARS\n"); break;
			case INS_STRI2INTS: fprintf(stderr, "INS_STRI2INTS\n"); break;

			case INS_READ: fprintf(stderr, "INS_READ\n"); break;
			case INS_WRITE: fprintf(stderr, "INS_WRITE\n"); break;

			case INS_CONCAT: fprintf(stderr, "INS_CONCAT\n"); break;
			case INS_STRLEN: fprintf(stderr, "INS_STRLEN\n"); break;
			case INS_GETCHAR: fprintf(stderr, "INS_GETCHAR\n"); break;
			case INS_SETCHAR: fprintf(stderr, "INS_SETCHAR\n"); break;

			case INS_TYPE: fprintf(stderr, "INS_TYPE\n"); break;

			case INS_LABEL: fprintf(stderr, "INS_LABEL\n"); break;
			case INS_JUMP: fprintf(stderr, "INS_JUMP\n"); break;
			case INS_JUMPIFEQ: fprintf(stderr, "INS_JUMPIFEQ\n"); break;
			case INS_JUMPIFNEQ: fprintf(stderr, "INS_JUMPIFNEQ\n"); break;
			case INS_JUMPIFEQS: fprintf(stderr, "INS_JUMPIFEQS\n"); break;
			case INS_JUMPIFNEQS: fprintf(stderr, "INS_JUMPIFNEQS\n"); break;

			case INS_EXIT: fprintf(stderr, "INS_EXIT\n"); break;

			case INS_BREAK: fprintf(stderr, "INS_BREAK\n"); break;
			case INS_DPRINT: fprintf(stderr, "INS_DPRINT\n"); break;

			//definovani vlastnich instrukci
			case PI_BEGINFUNC: fprintf(stderr, "PI_BEGINFUNC\n"); break;
			case PI_ENDFUNC: fprintf(stderr, "PI_ENDFUNC\n"); break;

			case PI_INIT: fprintf(stderr, "PI_INIT\n"); break;
			case PI_LTE: fprintf(stderr, "PI_LTE\n"); break;
			case PI_GTE: fprintf(stderr, "PI_GTE\n"); break;
			case PI_NEQ: fprintf(stderr, "PI_NEQ\n"); break;
			case PI_ADDSTR: fprintf(stderr, "PI_ADDSTR\n"); break;
			case PI_ADD: fprintf(stderr, "PI_ADD\n"); break;
			case PI_ASS: fprintf(stderr, "PI_ASS\n"); break;
			case PI_ASS_FUNC: fprintf(stderr, "PI_ASS_FUNC\n"); break;
			case PI_ASS_DECL: fprintf(stderr, "PI_ASS_DECL\n"); break;
			case PI_ASS_DECL_FUNC: fprintf(stderr, "PI_ASS_DECL_FUNC\n"); break;
			case PI_DEF_PARAM: fprintf(stderr, "PI_DEF_PARAM\n"); break;
			case PI_FCALL: fprintf(stderr, "PI_FCALL\n"); break;
			case PI_FCALL_PARAMT: fprintf(stderr, "PI_FCALL_PARAMT\n"); break;
			case PI_FCALL_PARAMID: fprintf(stderr, "PI_FCALL_PARAMID\n"); break;
			case PI_IF_START: fprintf(stderr, "PI_IF_START\n"); break;
			case PI_IF_ELSE: fprintf(stderr, "PI_IF_ELSE\n"); break;
			case PI_IF_END: fprintf(stderr, "PI_IF_END\n"); break;
			case PI_WHILE_START: fprintf(stderr, "PI_WHILE_START\n"); break;
			case PI_WHILE_EX: fprintf(stderr, "PI_WHILE_EX\n"); break;
			case PI_WHILE_END: fprintf(stderr, "PI_WHILE_END\n"); break;
			case PI_BUILTFUNC: fprintf(stderr, "PI_BUILTFUNC\n"); break;
			case PI_WHILE_DECL: fprintf(stderr, "PI_WHILE_DECL\n"); break;
		}

		temp = temp->next;
	}
}
