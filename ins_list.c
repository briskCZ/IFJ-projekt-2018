#include "ins_list.h"


int insertLast(t_Ins ins);
int insertBeforeActive(t_Ins ins);

// vytvori list pro instrukce
// @return t_InsList
void listInit(){
	
	list->first = NULL;
	list->act = NULL;
	list->last = NULL;
}

void setActive(t_Elem *elem)
{
	list->act = elem;
}

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

int insertLast(t_Ins ins)
{
	if (list == NULL)
	{
		fprintf(stderr, "ERROR: istlist neexistuje\n");
		exit(1);
	}
	
	t_Elem *new = malloc(sizeof(t_Elem));
	if (new == NULL)
		return MEMORY_ERROR;

	new->data = ins;
	new->next = NULL;
	new->prev = list->last;
	
	if (list->first == NULL || list->last == NULL)
	{
		list->first = list->last = list->first ins;
	}
	else //pokud neni list prazdny
	{
		list->last->next = new;
		list->last = new;
	}
	
	return MEMORY_OK;
}


int insertBeforeActive(t_Ins ins)
{
	if (list->act == NULL)
	{
		fprintf(stderr, "ERROR: list neni aktivni\n");
		exit(1);
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
	list->act->prev = new;
}

void rewriteActive(t_Ins *ins)
{
	if (list->act == NULL)
	{
		fprintf(stderr, "ERROR: list neni aktivni\n");
		exit(1);
	}
	list->act->ins_type = ins->ins_type;
	list->act->adr1 = ins->adr1;
	list->act->adr2 = ins->adr2;
	list->act->adr3 = ins->adr3;
}

void freeList()
{
	while(list->first != NULL)
	{
		t_Elem *temp = list->first;
		list->first = list->first->next;
		free(temp);
	}	
	list->act = list->last = NULL;
}