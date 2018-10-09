/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Implementace funkci pro string
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "string.h"

#define LEX_ERR 1
/* Definice navratovych hodnot */
#define ID 0
#define EQ_REL 1
#define ASSIGNMENT 2
#define KW 3

/* Datovy typ token */
typedef struct {
    int type;
    string attr;
} t_Token;

/*
Inicializaci scanneru
*/
void scannerInit();

/*
Cisteni po scanneru
*/
void scannerClean();
/*
*/
t_Token getNextToken();
