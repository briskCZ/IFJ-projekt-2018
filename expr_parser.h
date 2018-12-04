/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Hlavickovy soubor k precedencni analyze vyrazu
*/
#ifndef EXPR_H
#define EXPR_H

#include "string.h"
#include "ret_vals.h"
#include "int_stack.h"
#include "scanner.h"
#include "generator.h"
#include "symtable.h"
#include "ins_list.h"
#include "token_array.h"

#define PT_SIZE 7 //velikost precedencni tabulky
#define PT_X 102 //prazdne misto v tabulce symbolu
#define PT_R 101 //symbol >
#define PT_E 100 //symbol =
#define PT_L -100 //symbol <
#define PT_END 13 //musi byt vetsi nez 12 moznych tokenu co muzou prijit
#define PT_E_RULE 103 //symbyl E 
#define NON_TYPE -104 //znak pro vysledek vyrazu, ktery name vysledek, typicky ma hodnotu BOOL

/*
zjisti jake pravidlo se ma pouzit
@param t_IStack zasobnik pro precedenci tabulku
@param int slouzi pro zjisteni typu vysledne vyrazu
@return int cislo pravidla
*/
int checkRule(t_IStack *stack, int *type);

/*
zjisti jestli symbol je koncovy($) nebo ne
@param int cislo symbolu
@return int 0 ne, jinak ano
*/
int isEnd(int val);

/*
@param t_Token nacteny token
@param t_Token dalsi nacteny token pokud se nemuzeme hned rozhodnout jestli budeme zpracovavat vyraz
@param t_symTable ukazatel na lokalni tabulku symbolu, pokud je null tak jsem na globalni urovni
@param usingTb jestli pouzivame druhy token
@param int vysledek vyrazu
@return t_Token nacteny token, ktery neni uz neni ve vyrazu
*/
t_Token exprParse(t_Token t, t_Token tb, t_symTable *local_table, int usingTb, int *return_type);

/*
prida instrukci do listu instrukci
@param t_IStack zasobnik pro precedencni tabulku
@param t_symTable ukazatel na lokalni tabulku symbolu, pokud je null tak jsem na globalni urovni
@param t_Token ktery dostaneme ze scanneru
*/
void addInitInstruction(t_IStack *s, t_symTable *local_table, t_Token b_token);


typedef enum{
    R_PLUS = 200,	// E --> E + E
    R_MINUS = 201,	// E --> E - E
    R_MUL = 202,	// E --> E *E
    R_DIV = 203,	// E --> E / E
    R_PAR = 204,	// E --> (E)
    R_ID = 205,		// E --> i
    R_LESS = 206,	// E --> E < E
    R_MORE = 207,	// E --> E > E
    R_LESSEQ = 208,	// E --> E <= E
    R_MOREEQ = 209,	// E --> E >= E
    R_EQ = 210,		// E --> E == E
    R_NEQ = 211		// E --> E != E
} expr_rules;

#endif
