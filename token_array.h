/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Pole tokenu
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

int tarrInit(t_TokenArr *ta);
int tarrAddToken(t_TokenArr *ta, t_Token token);
t_Token tarrGetNextToken(t_TokenArr *ta);
t_Token tarrGetPrintNextToken(t_TokenArr *ta);
int tarrReturnToken(t_TokenArr *ta);
void tarrPrint(t_TokenArr *ta);
void tarrFree(t_TokenArr *ta);
int tarrFill(t_TokenArr *ta);

#endif
