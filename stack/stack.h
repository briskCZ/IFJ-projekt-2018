/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Hlavickovy soubor pro abstraktni datovy typ zasobnik
*/

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

#include "ret_vals.h"

/* data v elementu*/
typedef struct
{
	int val; // TODO
	//???

} t_Data;

/* element v zasobniku */
typedef struct node
{
	t_Data *data; // data
	struct node *next; // ukazatel na dalsi element

} t_Node;

/* zasobnik */
typedef struct
{
	t_Node *top; // ukazatel na vrchol zasobniku

} t_Stack;

/*
inicializace zasobiku
*/
t_Stack stackInit();

/*
zruseni zasobniku

@param s ukazatel na vrchol zasobniku
*/
void stackDestroy(t_Stack *s);

/*
vlozi element na vrchol zasobniku

@param s ukazatel na vrchol zasobniku
@param data date ktera se maji vlozit na zasobnik
@return v uspechu vrati MEMORY_OK v opacnem pripade MEMORU_ERROR
*/
int push(t_Stack *s, t_Data data); // data ?? TODO

/*
odstrani element z vrcholu zasobniku

@param s ukazatel na vrchol zasobniku
*/
void pop(t_Stack *s);

/*
vrati obsah dat na vrchulu zasobniku

@param s ukazatel na vrchol zasobniku
@return t_Data data, ktere jsou ulozeny na vrcholu zasobniku
*/
t_Data* top(t_Stack *s);

/*

@param s ukazatel na vrchol zasobniku
@return int 1 kdyz je prazdnu; 0 kdyz neni prazdny
*/
int isEmpty(t_Stack *s);


/*-------------------------
 * 		extra funkce 
 *-------------------------*/

/*
vypise obsah zasobniku

@param s ukazatel na vrchol zasobniku
*/
void display(t_Stack *s);

/*
vypocita hloubku zasobniku

@param s ukazatel na vrchol zasobniku
@return int hloubka zasobniku
*/
int depth(t_Stack *s);

#endif //STACK_H
