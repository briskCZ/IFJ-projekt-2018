/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Hlavickovy soubor pro parser;=
*/

#include "expr_parser.h"
#include "scanner.h"

void code(t_Token token);
void sec1();
void sec2();
void program();
void param1();
void param11();
void param2();
void param22();
void assign();
void f_call(t_Token ta, t_Token tb);
void term(t_Token token);
int isNextEol();
