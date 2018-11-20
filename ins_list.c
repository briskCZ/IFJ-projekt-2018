#include "ins_list.h"


int insertLast(t_Ins *ins);
int insertBeforeActive(t_Ins *ins);


//extern t_InsList *list;

// vytvori list pro instrukce
// @return t_InsList
int listInit(){

	list = malloc(sizeof(t_InsList));
	if (list == NULL)
		return MEMORY_ERROR;

	list->first = NULL;
	list->act = NULL;
	list->last = NULL;
	return MEMORY_OK;
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

int insertLast(t_Ins *ins)
{
	if (list == NULL)
	{
		fprintf(stderr, "ERROR: inslist neexistuje\n");
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
		list->first = list->last = new;
	}
	else //pokud neni list prazdny
	{
		list->last->next = new;
		list->last = new;
	}
	
	return MEMORY_OK;
}


int insertBeforeActive(t_Ins *ins)
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
	else
	{
		list->first = new;
	}
	list->act->prev = new;

	return MEMORY_OK;
}

void rewriteActive(int ins_type, void *adr1, void *adr2, void *adr3)
{
	if (list->act == NULL)
	{
		fprintf(stderr, "ERROR: list neni aktivni\n");
		exit(1);
	}

	list->act->data->ins_type = ins_type;
	list->act->data->adr1 = adr1;
	list->act->data->adr2 = adr2;
	list->act->data->adr3 = adr3;
}

void deleteFirst()
{
	if (list->first == list->act)
	{
		list->act = NULL;
	}
	t_Elem *temp = list->first;
	list->first = list->first->next;
	list->first->prev = NULL;

	free(temp->data);
	free(temp);
}

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

void printList()
{
	t_Elem *temp = list->first;
	
	while (temp != NULL)
	{
		fprintf(stderr, "%d\n", temp->data->ins_type);
		
		temp = temp->next;
	}
}
