/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Hlavickovy soubor pro parser;=
*/

#ifndef PARSER_H
#define PARSER_H

#include "expr_parser.h"
#include "scanner.h"
#include "symtable.h"
#include "ins_list.h"
#include "token_array.h"

struct table *pa_funcLocalTable; //NULL pokud je global scope, jinak v definici funkce


/*
t_symTable table; //globalni tabulka symbolu
t_Node *node; //pomocny ukazatel na data do tabulky symbolu
*/

int isGlobal();
t_Node* isAssignable(t_Token left, int type, int *isNew, t_symTable *scope);
void f_callAssIns(int isNew, t_Node *left, t_Node *right);
void assIns(int isNew, t_Node *left);
void paramHandler(t_Token token, int param_cnt);
void code(t_Token token);
void sec1();
void sec2();
void program();
void param1(int *param_cnt);
void param11(int *param_cnt);
void param2(t_Token token, int *param_cnt);
void param22(int *param_cnt);
void assign(t_Token left);
void f_call(t_Token ta, t_Token tb);
void term(t_Token token);
int isNextEol();


#endif //PARSER_H
