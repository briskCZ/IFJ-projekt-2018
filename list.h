/*  IFJ Project 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *
 *  Authors:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *
 *  Faculty of information technology BUT
 *  Description:
 *      Header file for double linked list structure, inspired by implementation in IAL
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct tItem {
    void data;
    struct tItem *next;
    struct tItem *prev;
} *tItemPtr;

typedef struct {
    tItemPtr First;
    tItemPtr Active;
    tItemPtr Last;
} tList;

void LInit(tList *);
void LDispose(tList *);
void LInsertFirst(tList *, int);
void LInsertLast(tList *, int);
//....
