/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Hlavickovy soubor pro dynamicke pole tokenu
*/

#include "scanner.h"
#include "symtable.h"

#ifndef TARR_H
#define TARR_H

#define TARR_MALLOC_SIZE 16

typedef struct{
    t_Token *array;
    int size;      //alokovana velikost
    int index;     //aktualni index v poli tokenu
    int max_index; //maximalni dosazeny index v poli
}t_TokenArr;

t_TokenArr token_array;  //pole pro ulozeni tokenu pro druhy pruchod
/*
Inicializace pole
@param ta - pole pro inicializaci
@return MEMORY_OK / MEMORY_ERROR
*/
int tarrInit(t_TokenArr *ta);
/*
Pridani polozky
@param ta - pole do ktereho se ma pridat
@param token - pridavana polozka
@return MEMORY_OK / MEMORY_ERROR
*/
int tarrAddToken(t_TokenArr *ta, t_Token token);
/*
Prvni pruchod, zpracuje informace o funkcich v celem souboru (nazev, pozet parametru)
a tyto informace ulozi do tabulky symbolu
@param ta - pole pro zpracovani
*/
void tarrGetFuncInfo(t_TokenArr *ta);
/*
Obdoba getNextToken, vrati dalsi token, zvysi se index v poli
@param ta - pole ze ktereho vrati dalsi token
@return t_Token - vraceny token
*/
t_Token tarrGetNextToken(t_TokenArr *ta);
/*
Debugovaci funkce, ktera provede tarrGetNextToken, ale vytiskne navic jeho obsah
*/
t_Token tarrGetPrintNextToken(t_TokenArr *ta);
/*
Snizi index v poli ta (vrati token)
@param ta - pole, kam se ma vratit token
@return - MEMORY OK / MEMORY_ERROR
*/
int tarrReturnToken(t_TokenArr *ta);
/*
Naplni pole vsemi dostupnymi tokeny po EOF, pokud se objevi token s lexikalni
chybou, automaticky ukoncuje program
@param ta - pole pro naplneni
@return - MEMORY_OK/ MEMORY_ERROR
*/
int tarrFill(t_TokenArr *ta);
/*
Uvolni pole
@param ta - pole pro uvolneni
*/
void tarrFree(t_TokenArr *ta);
/*
Debugovaci funkce pro vypis pole
@param ta - pole, ktere se ma vypsat
*/
void tarrPrint(t_TokenArr *ta);

#endif
