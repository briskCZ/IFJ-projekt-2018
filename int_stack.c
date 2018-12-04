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


    node->sym = sym;
	node->type = type;
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
int i_topPop(t_IStack *s, int *type)
{
    int sym = i_top(s, type);
    i_pop(s);
    return sym;
}

// vrati data z vrcholu zasobniku
int i_top(t_IStack *s, int *type)
{
	if (i_isEmpty(s))
		return STACK_ERROR;

	*type = s->top->type;
	return s->top->sym;
}

//vrati nejvysi terminal na zasobniku
int i_termTop(t_IStack *s, int *type){
	if (i_isEmpty(s))
		return STACK_ERROR;

	t_INode *node = s->top;

	//tokeny 0 az 13
	while(node->sym < 0 || node->sym > 13)
	{
		if (node->sym == T_INT || node->sym == T_FLOAT || node->sym == T_STRING || node->sym == T_NIL)
			break;
		node = node->next;
		if (node == NULL) return STACK_ERROR;
	}
	
	*type = node->type;
	return node->sym;
}

//vlozi symbol pres nejvyssi terminal na zasobniku
int i_termTopPush(t_IStack *s, int sym, int type){
	if (i_isEmpty(s))
		return STACK_ERROR;

	t_INode *new = malloc(sizeof(t_INode));
	if (new == NULL) 
		return MEMORY_ERROR;
	new->sym = sym;
	new->type = type;

	t_INode *node = s->top;
	t_INode *prev = NULL;
	
	
	//tokeny 0 az 13
	while(node->sym < 0 || node->sym > 13)
	{
		if (node->sym == T_INT || node->sym == T_FLOAT || node->sym == T_STRING || node->sym == T_NIL)
			break;
			
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

/*--------------------------*/
/*	 debugovaci funkce 		*/
/*--------------------------*/

// vypise obsah zasobniku
void i_display(t_IStack *s)
{
    t_IStack temp;
    temp = *s;
    fprintf(stderr, "TOP: ");
    while (!i_isEmpty(&temp))
    {
        fprintf(stderr, "[%d %d] |", temp.top->sym, temp.top->type);
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
