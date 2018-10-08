/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Hlavickovy soubor pro abstraktni datovy typ string
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_ALLOC_INC 8    //blok po kterem se ma alokovat pamet pro s->val
#define STR_ERROR 1         //pokud se neco interne nepodari
#define STR_SUC 0           //success

/* Struktura string */
typedef struct {
    char *val;              //hodnota stringu
    int length;             //aktualni delka stringu
    int allocated_size;     //alokovana pamet
} string;

/*
Inicializuje string
@param s - string urceny pro inicializaci

@return STR_ERROR nebo STR_SUC podle uspesnosti
*/
int stringInit(string *s);

/*
Uvolni string
@param s - string urceny pro uvolneni

@return STR_ERROR nebo STR_SUC podle uspesnosti
*/
void stringFree(string *s);

/*
Vlozi do stringu retezec
@param s - cilvoy string
@param str - vstupni C-type string (char *)

@return STR_ERROR nebo STR_SUC podle uspesnosti
*/
int stringInsert(string *s, char *str);

/*
Vycisti obsah stringu, ovsem necha alokovanou pamet
@param s - string, ktery chceme vyctisti
*/
void stringClear(string *s);
/*
Vlozi do stringu char
@param s - cilovy string
@param c - znak, urceny pro vlozeni

@return STR_ERROR nebo STR_SUC podle uspesnosti
*/
int stringAddChar(string *s, char c);
/*
Porovna dva stringy
@param sa - prvni string
@param sb - druhy string

@return - stejny jako strcmp()
*/
int stringCompare(string *sa, string *sb);

/*
Vrati delku stringu
@param s - string

@return - delka stringu
*/
int stringGetLength(string *s);

/*
Vrati s->val (retezec se struktury string)
@param s - string

@return char* - cckovy retezec z s->val
*/
char * stringGet(string *s);


/*
Debugovaci funkce - vytiskne string
*/
void stringPrint(string *s);
