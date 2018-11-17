/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Hlavickovy soubor k precedencni analyze vyrazu
*/

#include "string.h"
#include "ret_vals.h"
#include "int_stack.h"
#include "scanner.h"

#define PT_SIZE 14
#define PT_X 102
#define PT_R 101
#define PT_E 100
#define PT_L -100
#define PT_END_INDEX 13
#define PT_E_RULE 103


int checkRule(t_IStack *stack);
int isEnd(int val);
int exprParse();

typedef enum{
    R_PLUS = 200,
    R_MINUS,
    R_MUL,
    R_DIV,
    R_PAR,
    R_ID,
    R_LESS,
    R_MORE,
    R_LESSEQ,
    R_MOREEQ,
    R_EQ,
    R_NEQ
} expr_rules;
