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
/*BEGIN Pouzivane jako indexy u exprParse*/
#define T_PLUS 0      // +
#define T_MINUS 1     // -
#define T_MUL 2       // *
#define T_DIV 3       // /
#define T_LESS 4      // <
#define T_LESS_EQ 5   // <=
#define T_MORE 6      // >
#define T_MORE_EQ  7  // >=
#define T_EQ_REL 8    // ==
#define T_NOT_EQ 9    // !=
#define T_LEFT_PAR 10  // (
#define T_RIGHT_PAR 11 // )
/* Identifikator */
#define T_ID 12
/* END indexy u exprParse */
#define T_ASSIGNMENT 13// =
#define T_COMMA 14    // ,
/* Klicova slova */
#define T_DEF 15
#define T_DO 16
#define T_ELSE 17
#define T_END 18
#define T_IF 19
#define T_NOT 20
#define T_THEN 21
#define T_WHILE 22
#define T_NIL 23

#define T_EOF -1
#define T_EOL 27
/* Typy */
#define T_INT 24
#define T_FLOAT 25
#define T_STRING 26
#define T_PARAM 28


/* Stavy pro konecny automat */
#define S_START 0
#define S_LINE_COMMENT 1
#define S_EQUALS 2
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
#define S_FLOAT 14
#define S_EXPONENT 15
#define S_OCTAL 16
#define S_BIN 17
#define S_HEX 18
#define S_ZERO 19
#define S_BC_END 20

#define INT_LENGTH 12

/* Makra pro zjednoduseni programu */
#define strAdc(a, b) if(stringAddChar(a, b) == MEMORY_ERROR){ *error = ERROR_INTERNAL; fprintf(stderr, "INTERNAL_ERROR: STR_ERROR: Memory\n");return sc_token;}
#define strCopy(a, b) if(stringCopy(a, b) == MEMORY_ERROR){ *error = ERROR_INTERNAL; fprintf(stderr, "INTERNAL_ERROR: STR_ERROR: Memory\n");return sc_token;}

/* Datovy typ token */
typedef struct {
    int type;
    string attr;
} t_Token;

/* Globalni promenne */
t_Token sc_mem_token; //vraceny token
string sc_buffer;  //buffer pro identifikatory a kw
string sc_aux_buffer; //pomocny buffer pro blokove komentare a hexa cisla
int sc_uab;   //zda pouzivat pomocny buffer
int sc_abi; //index v pomocnem bufferu
int sc_line_cnt; //radek v souboru
int sc_using_mem_token; //zda pouzivame vraceny token
int sc_was_eol; //pokud byl eol
t_Token sc_token;

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
Test zda znak, muze byt validni konec cisla
@param c - testovany znaky

@return - vraci 1 pokud je konec cisla mozny, jinak 0
*/
int isNumberEnding(char c);

/*
Testuje dalsi znak ze stdin, zda je to mozny ukoncujici znak KW
@return - vraci 1 pokud je konec kw, jinak 0
*/
int isKwEnd();

/*
Funkce testujici, jestli je zacatek/konec komentare

@return -   pri uspechu nenulova hodnota, jinak 0 a (begin)zapise prectene znaky do sc_aux_buffer
            a nastavi sc_uab na 1, ay dale byly znaky z tohoto bufferu nacitany (/begin)

*/
int isCmntBegin(char symbol);
int isCmntEnd(char *sym);
/*
Zadost o dalsi token
@param error - hodnotou predavany error ERROR_INTERNAL / ERROR_LEX

@return vraci vyhodnoceny token
*/
t_Token getNextToken(int *error);

/*
Funkce pro ladeni
vytiskne a vrati dalsi token
*/
t_Token getPrintNextToken(int *error);

/*
Vrati token scanneru
@param t - vraceny token
@return - uspesnost operace MEMORY_ERROR, pokud vracime vic nez 1 token
*/
int returnToken(t_Token t);

/*
Prevede string s na interni reprezentaci IFJcode18
@param s - prevadeny string
*/
void stringToIns(string *s);
#endif
