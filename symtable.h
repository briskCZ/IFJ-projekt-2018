/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Hlavickovy soubor pro abstraktni datovy typ VBS - vyhledavaci binarni strom
*/

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>

#include "string.h"
#include "scanner.h"

typedef struct table t_symTable;

/* data uzlu*/
typedef struct
{
	int is_var; //0 - funkce, jinak promenna
	string *name; // identifikator promenne
	int data_type; // datovy typ
	int defined;
	int global; // 0 lokalni, jinak globalni
	t_symTable *local_symTable; //lokalni tabulka symbolu

} t_Data;

/* uzel */
typedef struct node
{
	t_Data *data; //data uzlu
	struct node *left; //levy syn
	struct node *right; //pravy syn

} t_Node;

/* tabulka */
struct table 
{
	t_Node *root; //pocatek tabulky

};

/*

*/
t_symTable tableInit();

/*

*/
void tableDestroy(t_symTable *t);

/*

*/
int tableInsertToken(t_symTable *table, t_Token token);

/*

*/
t_Node* tableSearchToken(t_symTable *table, t_Token token);

#endif //SYMTABLE_H
