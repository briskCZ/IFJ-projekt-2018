/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Implementace funkci pro zasobnik
*/

#include "int_stack.h"

// inicializuje zasobnik
t_IStack i_stackInit()
{
	t_IStack s;
	s.top = NULL;

	return s;
}

// zrusi zasobnik
void i_stackDestroy(t_IStack *s)
{
	while(!i_isEmpty(s))
		i_pop(s);
}

// vlozi element na vrchol zasobniku
int i_push(t_IStack *s, int sym, int type)
{
    t_INode *node = malloc(sizeof(t_INode));
    if (node == NULL)
       return MEMORY_ERROR;

	t_IData *data = malloc(sizeof(t_IData));
	if (data == NULL)
	{
		free(node);
		return MEMORY_ERROR;
	}

    node->data->sym = sym;
	node->data->type = type;
	
	node->next = s->top;
    s->top = node;

	return MEMORY_OK;
}

// odstrani element z vrcholu zasobniku
void i_pop(t_IStack *s)
{
	if (i_isEmpty(s))
		return ;

	t_INode *node = s->top;
	s->top = s->top->next;
	
	free(node->data);
	free(node);
}
//vrati a odstrani hodnotu z vrcholu zasobniku
t_IData *i_topPop(t_IStack *s)
{
    t_IData *data = i_top(s);
    i_pop(s);
    return data;
}

// vrati data z vrcholu zasobniku
t_IData *i_top(t_IStack *s)
{
	if (i_isEmpty(s))
		return STACK_ERROR;

	return s->top->data;
}

// vrati data termu
t_IData *i_termTop(t_IStack *s){
	if (i_isEmpty(s))
		return NULL;

	t_INode *node = s->top;
	//tokeny 0 az 13
	while(!(node->data >= 0 && node->data <= 13)){
		node = node->next;
		if (node == NULL) return NULL;
	}
	return node->data;
}

//vlozi data pred term
 int i_termTopPush(t_IStack *s, int sym, int type){
	if (i_isEmpty(s))
		return STACK_ERROR;

	t_INode *node = malloc(sizeof(t_INode));
    if (node == NULL)
       return MEMORY_ERROR;

	t_IData *data = malloc(sizeof(t_IData));
	if (data == NULL)
	{
		free(node);
		return MEMORY_ERROR;
	}
	
	new->data->sym = sym;
	new->data->type = type;

	t_INode *node = s->top;
	t_INode *prev = NULL;
	//tokeny 0 az 13
	while(!(node->data >= 0 && node->data <= 13)){
		prev = node;
		node = node->next;
		if (node == NULL) return STACK_ERROR;
	}

	if (prev == NULL)
	{
		new->next = s->top;
		s->top = new;
	}
	else
	{
		new->next = node;
		prev->next = new;
	}

	return MEMORY_OK;
}

/* test jestli zasobnik je prazdny
- pokud ano vraci 1 - true
- pokud ne  vraci 0 - false*/
int i_isEmpty(t_IStack *s)
{
	return s->top == NULL ? 1 : 0;
}

// vypise obsah zasobniku
void i_display(t_IStack *s)
{
    t_IStack temp;
    temp = *s;
    fprintf(stderr, "TOP: ");
    while (!i_isEmpty(&temp))
    {
        fprintf(stderr, "%d, %d |", temp.top->data->sys, temp.top->data->type);
        temp.top = temp.top->next;
    }

    fprintf(stderr,"\n");
}

// vypocita hloubku zasobniku
int i_depth(t_IStack *s)
{
    int counter = 0;

    t_IStack *temp;
    temp = s;

    while (!i_isEmpty(temp))
    {
        counter++;
        temp->top = temp->top->next;
    }

    return counter;
}
