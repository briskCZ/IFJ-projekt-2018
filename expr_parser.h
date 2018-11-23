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

#define PT_SIZE 7
#define PT_X 102
#define PT_R 101
#define PT_E 100
#define PT_L -100
#define PT_END 13 //musi byt vetsi nez 12 moznych tokenu co muzou prijit
#define PT_E_RULE 103
#define NON_TYPE -104

int checkRule(t_IStack *stack, int *type);
int isEnd(int val);
t_Token exprParse(t_Token t, t_Token tb, t_symTable *local_table, int usingTb, int *return_type);
void addInitInstruction(t_IStack *s, t_symTable *local_table, t_Token b_token);

typedef enum{
    R_PLUS = 200,
    R_MINUS = 201,
    R_MUL = 202,
    R_DIV = 203,
    R_PAR = 204,
    R_ID = 205,
    R_LESS = 206,
    R_MORE = 207,
    R_LESSEQ = 208,
    R_MOREEQ = 209,
    R_EQ = 210,
    R_NEQ = 211
} expr_rules;

#endif
