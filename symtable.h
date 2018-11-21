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
#include "parser.h"
//#include "param_array.h"

#define MAX_SIZE 8 //

typedef struct table t_symTable;
typedef struct node t_Node;
typedef struct arrParam t_ArrParams;

t_symTable table; //globalni tabulka symbolu
t_Node *node; //pomocny ukazatel na data do tabulky symbolu

/* data uzlu*/
typedef struct
{
	int is_var; //0 - funkce, jinak promenna
	string *name; // identifikator promenne
	int data_type; // datovy typ
	int defined; //  0 - neni definovana, jinak je definovana
	int global; // 0 lokalni, jinak globalni
	int params_cnt;
	t_ArrParams *arr_params; //TODO
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

/* pole parametru */
typedef struct arrParam
{
	
	int size_alloc;
	string *string;
	

} t_ArrParams;

int arrParamInit(t_Data *data);
int arrParamAdd(t_Data *data, string s);
void arrParamFree(t_Data *data);

/*

*/
t_symTable tableInit();

/*

*/
void tableDestroy();

/*

*/
int tableInsertToken(t_Token token);

/*

*/
t_Node* tableSearchItem(string s);

#endif //SYMTABLE_H
