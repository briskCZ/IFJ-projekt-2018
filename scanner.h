/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Hlavickovy soubor scanneru
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "string.h"
#include "ret_vals.h"

#ifndef SCANNER_H
#define SCANNER_H

/* Definice navratovych hodnot */
#define EQ_REL 1    // ==
#define ASSIGNMENT 2// =
#define MUL 3       // *
#define DIV 4       // /
#define PLUS 5      // +
#define MINUS 6     // -
#define LEFT_PAR 7  // (
#define RIGHT_PAR 8 // )
#define LESS 9      // <
#define MORE 10     // >
#define LESS_EQ 11  // <=
#define MORE_EQ 12  // >=
#define NOT_EQ 13   // !=
#define STR 14      // ""
#define COMMA 15    // ,
/* Klicova slova */
#define NIL 0
#define DEF 16
#define DO 17
#define ELSE 18
#define END 19
#define IF 20
#define NOT 21
#define THEN 22
#define WHILE 23

#define T_EOF -1
/* Identifikator */
#define ID 24
/* Typy */
#define INT 25
#define DOUBLE 26
#define STRING 27


/* Stavy pro konecny automat */
#define S_START 0
#define S_LINE_COMMENT 1
#define S_EQUALS 2
#define S_BC_BEGIN 3
#define S_BLOCK_COMMENT 4
#define S_ID_KW 5
#define S_LESS 6
#define S_MORE 7
#define S_NOT_EQ 8
#define S_STRING 9
#define S_SPECIAL_SYMBOL 10
#define S_SPECIAL_HEX 11
#define S_DIGIT 12
#define S_INT 13
#define S_DOUBLE 14
#define S_EXPONENT 15
#define S_BC_END 16

/* Makra pro zjednoduseni programu */
#define strAdc(a, b) if(stringAddChar(a, b) == STR_ERROR){ *error = ERROR_INTERNAL; fprintf(stderr, "INTERNAL_ERROR: STR_ERROR: Memory\n");return sc_token;}
#define strCopy(a, b) if(stringCopy(a, b) == STR_ERROR){ *error = ERROR_INTERNAL; fprintf(stderr, "INTERNAL_ERROR: STR_ERROR: Memory\n");return sc_token;}

/* Datovy typ token */
typedef struct {
    int type;
    string attr;
} t_Token;


/* Vyprintuje token */
void printToken(t_Token t, int error);

/* Spousteci a koncici funkce scanneru */
int scannerInit();
void scannerClean();

/*
Test zda charakter odpovida hexa formatu
@param c - testovany charakter

@return - vraci 1 pokud je charakter validni, jinak 0
*/
int isValidHex(char c);

/*
Vrati znak reprezentovany v sestnactkove soustave
@param s - string, ve kterem je reprezentovanem

@return - konvertovany znak
*/
int hexToChar(string *s);

/*
Test zda znak, muze byt validni konec cisla
@param c - testovany znaky

@return - vraci 1 pokud je konec cisla mozny, jinak 0
*/
int isNumberEnding(char c);

/*
Zadost o dalsi token
@param error - hodnotou predavany error

*/
t_Token getNextToken(int *error);
#endif
