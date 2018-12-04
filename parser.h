/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Hlavickovy soubor pro parser
*/

#ifndef PARSER_H
#define PARSER_H

#include "expr_parser.h"
#include "scanner.h"
#include "symtable.h"
#include "ins_list.h"
#include "token_array.h"

struct table *pa_funcLocalTable; //NULL pokud je global scope, jinak v definici funkce

int pa_while = 0;           //ve while = 1, mimo while = 0
int pa_if = 0;              //v if = 1, mimo if = 0
int pa_if_count = 0;        //pocitadlo zanoreni v ifu
int pa_while_count = 0;     //pocitadlo zanoreni while

/*
Funkce pro zjisteni aktualniho scope
@return - 1 pokud je globalni scope, 0 pokud jsme ve funkci
*/
int isGlobal();

/*
Zajistuje semantickou kontrolu, zda je mozne to promene left priradit
Prohleda tabulky symbolu a vrati spravonu levou stranu pro prirazeni
@param left - token leve promenne
@param type - typ tokenu
@param isNew - odkaz zda vyvarena hodnota byla nalezena nebo je vytvorena
@param scope - aktualni scope
@return - node v tabulce symbolu, do ktereho je prirazovano
*/
t_Node* isAssignable(t_Token left, int type, int *isNew, t_symTable *scope);
/*
Funkce pro generovani 3AD kodu - ridi se take globalnimi promennymi pa_while a
pa_if z duvodu deklarace pred whilem/ifem
@param isNew - urcuje zda se ma promena deklarovat
@param left - ukazatel do tabulky symbolu na levou stranu
@param right - u prirazeni volani funkce, aby se vedelo jakou funkci se ma volat
*/
void f_callAssIns(int isNew, t_Node *left, t_Node *right);
void assIns(int isNew, t_Node *left);
/*
Funkce kontrolujici spravnost parametru
@param token - token parametru
@param param_cnt - kolikaty je parametr
*/
void paramHandler(t_Token token, int param_cnt);
void paramDefHandler(t_Token token, int param_cnt);
/* Jednotlive funkce rekurzivniho sestupu */
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
/*
Dealokace pameti
*/
void cleanAll();


#endif //PARSER_H
