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

#define MAX_SIZE 8 //pocet alokovanych prvku pro parametry funkce

//typedef struct table t_symTable;
typedef struct node t_Node;
typedef struct arrParam t_ArrParams;

/* tabulka symbol */
typedef struct table
{
	t_Node *root; //pocatek tabulky

}t_symTable;

/* data v uzlu*/
typedef struct
{
	int is_var; //0 - funkce, jinak promenna
	string *name; // identifikator promenne
	int data_type; // datovy typ
	int defined; //  0 - neni definovana, jinak je definovana
	int global; // 0 lokalni, jinak globalni
	int params_cnt; // pocet parametru funkce
	int was_called;	//zda byla volana kdyz nebyla definovana
	int params_call_cnt; //TODO
	t_ArrParams *arr_params; //identifikatory parametru funkce
	t_symTable *local_symTable; //lokalni tabulka symbolu

} t_Data;

/* uzel v tabulce symbolu */
struct node
{
	t_Data *data; //data uzlu
	struct node *left; //levy syn
	struct node *right; //pravy syn

};

/* pole parametru */
struct arrParam
{

	int size_alloc;
	string *string;


};

t_symTable table; //globalni tabulka symbolu
t_Node *node; //pomocny ukazatel na data do tabulky symbolu

/*

*/
t_symTable tableInit();

/*

*/
void tableDestroy(t_symTable *table);

/*

*/
t_Node* tableInsertToken(t_symTable *table, t_Token token);

/*

*/
t_Node* tableSearchItem(t_symTable *table, string s);

void tableChangeItemByNode(t_Node *node, int is_var, int data_type, int defined, int global);
void tableChangeItemByString(t_symTable *table, string *s, int is_var, int data_type, int defined, int global);
/*

*/
int arrParamInit(t_Data *data);

/*

*/
int arrParamAdd(t_Data *data, string s);

/*

*/
void arrParamFree(t_Data *data);

/*

*/
void arrPrintParam(t_Data *data);

/* debugovaci funkce */

//vypise tabulku symbolu
void tablePrint(t_symTable *table, int local);

//vytvori token a vlozi to tabulky symbolu
void sInsert(t_symTable *table, int itype, char *is);

#endif //SYMTABLE_H
