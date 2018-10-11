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

#define LEX_ERR 1
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

/* Identifikator */
#define ID 24
/* Typy */
#define INT 25
#define DOUBLE 26
#define STRING 27


/* Stavy */
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
#define S_DIGIT 10

/* Datovy typ token */
typedef struct {
    int type;
    char* attr;
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

t_Token getRestToken();
t_Token getNextToken();
