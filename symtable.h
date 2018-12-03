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

typedef struct table t_symTable;
typedef struct node t_Node;
//typedef struct arrParam t_ArrParams;


t_symTable table; //globalni tabulka symbolu
t_Node *node; //pomocny ukazatel na data do tabulky symbolu

/* data v uzlu*/
typedef struct
{
	int is_var; //0 - funkce, jinak promenna
	string *name; // identifikator promenne
	int data_type; // datovy typ
	int defined; //  0 - neni definovana, jinak je definovana
	int global; // 0 lokalni, jinak globalni
	int params_cnt; // pocet parametru funkce
	int was_found;	//zda byla volana kdyz nebyla definovana
	t_symTable *local_symTable; //lokalni tabulka symbolu

} t_Data;

/* uzel v tabulce symbolu */
struct node
{
	t_Data *data; //data uzlu
	struct node *left; //levy syn
	struct node *right; //pravy syn

};

/* tabulka symbol */
typedef struct table
{
	t_Node *root; //pocatek tabulky

};

/*
inicializuje tabulku symbolu
@return t_symTable inicializovana tabulka symbolu
*/
t_symTable tableInit();

/*
uvolni tabulky symbolu
@param t_symTable ukazatel na tabulku symbolu, ktera se ma uvolnit
*/
void tableDestroy(t_symTable *table);

/*
vlozi token do tabulky symbolu
@param t_symTable ukazatel na tabulku symbolu
@param t_Token vkladany token do tabulky symbolu
@return t_Node ukazatel na uzel prave vytvoreny
*/
t_Node* tableInsertToken(t_symTable *table, t_Token token);

/*
hleda v tabulce sombolu podle stringu
@param t_symTable tabulka symbolu
@param string klic
@return t_Node ukazatel na uzel v tabulce symboly
*/
t_Node* tableSearchItem(t_symTable *table, string s);

/*
zmeni obsah tabulky symbolu
@param t_Node ukazatel na uzel v tabulce symbolu
@@param int jestli se jedna o promennou, 0 neni to promena 
@param int datovy typ
@param int jestli je definovana
@param int jestli je globalniparam int
*/
void tableChangeItemByNode(t_Node *node, int is_var, int data_type, int defined, int global);

/*
zmeni obsah tabulky symbolu
@param t_symTable table ve ktere se bude hledat
@param string klic
@param int jestli se jedna o promennou, 0 neni to promena 
@param int datovy typ
@param int jestli je definovana
@param int jestli je globalni
*/
void tableChangeItemByString(t_symTable *table, string *s, int is_var, int data_type, int defined, int global);

/*-------------------------------*/
/* 		debugovaci funkce 		 */
/*-------------------------------*/

//vypise tabulku symbolu
void tablePrint(t_symTable *table, int local);
//vypise node v tabulce symbolu
void tablePrintItem(t_Node *node);
//vytvori token a vlozi to tabulky symbolu
void sInsert(t_symTable *table, int itype, char *is);

#endif //SYMTABLE_H
