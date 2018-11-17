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
int i_push(t_IStack *s, int data)
{
    t_INode *node = malloc(sizeof(t_INode));
    if (node == NULL)
       return MEMORY_ERROR;


    node->data = data;
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
	free(node);
}
//vrati a odstrani hodnotu z vrcholu zasobniku
int i_topPop(t_IStack *s)
{
    int ret = i_top(s);
    i_pop(s);
    return ret;
}

// vrati data z vrcholu zasobniku
int i_top(t_IStack *s)
{
	if (i_isEmpty(s))
		return STACK_ERROR;

	return s->top->data;
}
int i_termTop(t_IStack *s){
	if (i_isEmpty(s))
		return STACK_ERROR;

	t_INode *node = s->top;
	while(!(node->data >= 0 && node->data <= 13)){
		node = node->next;
		if (node == NULL) return STACK_ERROR;
	}
	return node->data;
}

int i_termTopPush(t_IStack *s, int val){
	if (i_isEmpty(s))
		return STACK_ERROR;

	t_INode *new = malloc(sizeof(t_INode));
	//TODO
	new->data = val;

	t_INode *node = s->top;
	t_INode *prev = NULL;
	while(!(node->data >= 0 && node->data <= 13)){
		printf("sf");
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

	return node->data;
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
        fprintf(stderr, "%d |", temp.top->data);
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
