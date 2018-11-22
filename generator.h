/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  					TODO
*/

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include "ret_vals.h"
#include "ins_list.h"
#include "scanner.h"


unsigned uniqueNum; //pocitadlo promennych
unsigned func_cnt; //pocitadlo funkci

/*
vygeneruje kod
@return int SUCCES pokud uspesne
*/
int generate();



#endif //GENERATOR_H