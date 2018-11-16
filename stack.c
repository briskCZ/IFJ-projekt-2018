/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Implementace funkci pro zasobnik
*/

#include "stack.h"

// inicializuje zasobnik
t_Stack stackInit()
{
	t_Stack s;
	s.top = NULL;

	return s;
}

// zrusi zasobnik
void stackDestroy(t_Stack *s)
{
	while(!isEmpty(s))
		pop(s);
}

// vlozi element na vrchol zasobniku
int push(t_Stack *s, t_Token *token)
{
    t_Node *node = malloc(sizeof(t_Node));
    if (node == NULL)
       return MEMORY_ERROR;

	node->data = malloc(sizeof(t_Token));
	if (node->data == NULL)
	{
		free(node);
		return MEMORY_ERROR;
	}

    node->data->type = token->type;
    if (stringCopy(&node->data->attr, & token->attr) == MEMORY_ERROR){
        return MEMORY_ERROR;
    }
	node->next = s->top;
    s->top = node;

	return MEMORY_OK;
}

// odstrani element z vrcholu zasobniku
void pop(t_Stack *s)
{
	if (isEmpty(s))
		return ;

	t_Node *node = s->top;
	s->top = s->top->next;
    stringFree(&node->data->attr);
	free(node->data);
	free(node);
}
//vrati a odstrani hodnotu z vrcholu zasobniku
t_Token* topPop(t_Stack *s)
{
    t_Token *ret = top(s);
    pop(s);
    return ret;

}

// vrati data z vrcholu zasobniku
t_Token* top(t_Stack *s)
{
	if (isEmpty(s)) //TODO test - ted nebo kdo vola funkci ??
		return NULL;

	return s->top->data;
}

/* test jestli zasobnik je prazdny
- pokud ano vraci 1 - true
- pokud ne  vraci 0 - false*/
int isEmpty(t_Stack *s)
{
	return s->top == NULL ? 1 : 0;
}

// vypise obsah zasobniku
void display(t_Stack *s)
{
    t_Stack temp;
    temp = *s;
    while (!isEmpty(&temp))
    {
        printToken(*temp.top->data, 0);
        temp.top = temp.top->next;
    }

    fprintf(stderr,"\n");
}

// vypocita hloubku zasobniku
int depth(t_Stack *s)
{
    int counter = 0;

    t_Stack *temp;
    temp = s;

    while (!isEmpty(temp))
    {
        counter++;
        temp->top = temp->top->next;
    }

    return counter;
}
