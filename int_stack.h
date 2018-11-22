/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Hlavickovy soubor pro abstraktni datovy typ zasobnik
*/

#ifndef ISTACK_H
#define ISTACK_H

#include <stdio.h>
#include <stdlib.h>

#include "ret_vals.h"
#include "scanner.h"



/* element v zasobniku */
typedef struct inode
{
	int sym; // data
	int type; //datovy typ
	struct inode *next; // ukazatel na dalsi element

} t_INode;

/* zasobnik */
typedef struct
{
	t_INode *top; // ukazatel na vrchol zasobniku

} t_IStack;

/*
inicializace zasobiku
*/
t_IStack i_stackInit();

/*
zruseni zasobniku

@param s ukazatel na vrchol zasobniku
*/
void i_stackDestroy(t_IStack *s);

/*
vlozi element na vrchol zasobniku

@param s ukazatel na vrchol zasobniku
@param data date ktera se maji vlozit na zasobnik
@return v uspechu vrati MEMORY_OK v opacnem pripade MEMORU_ERROR
*/
int i_push(t_IStack *s, int sym, int type);

/*
vlozi element na vrchol zasobniku

@param s zasobnik
@param sym indexy u precedenci tabulky
@param type datovy typ
@return int jestli se povedlo vlozit prvek
*/
int i_termTopPush(t_IStack *s, int sym, int type);

/*
odstrani element z vrcholu zasobniku

@param s ukazatel na vrchol zasobniku
*/
void i_pop(t_IStack *s);

/*
Vrati a odstrani token z vrcholu zasobniku

@param s ukazatel na vrchol zasobniku
@param type vrati a odstrani datovy typ ktery je na vrcholu zasobniku
@return int vrati a odstrani symbol ktery je na vrcholu zasobniku
*/
int i_topPop(t_IStack *s, int *type);

/*
vrati obsah dat na vrchulu zasobniku

@param s ukazatel na vrchol zasobniku
@param type vrati datovy typ ktery je na vrcholu zasobniku
@return int vrati symbol ktery je na vrcholu zasobniku
*/
int i_top(t_IStack *s, int *type);

/*
vrati data prvniho termu na zasobniku

@param s ukazatel na vrchol zasobniku
@param type vrati datovy typ termu ktery je nejbliz vrcholu zasobniku
@return int vrati symbol, ktery je nejbliz vrcholu zasobniku
*/
int i_termTop(t_IStack *s, int *type);

/*

@param s ukazatel na vrchol zasobniku
@return int 1 kdyz je prazdnu; 0 kdyz neni prazdny
*/
int i_isEmpty(t_IStack *s);

/*-------------------------
 * 		extra funkce
 *-------------------------*/

/*
vypise obsah zasobniku

@param s ukazatel na vrchol zasobniku
*/
void i_display(t_IStack *s);

/*
vypocita hloubku zasobniku

@param s ukazatel na vrchol zasobniku
@return int hloubka zasobniku
*/
int i_depth(t_IStack *s);

#endif //STACK_H
