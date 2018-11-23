/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  					TODO
*/

#ifndef INSLIST_H
#define INSLIST_H

#include "ret_vals.h"
#include <stdlib.h>
#include <stdio.h>

//define instrukci pro generovani kodu
#define INS_MOVE 0
#define INS_CREATEFRAME 1
#define INS_PUSHFRAME 2
#define INS_POPFRAME 3
#define INS_DEFVAR 4
#define INS_CALL 5
#define INS_RETURN 6

#define INS_PUSHS 7
#define INS_POPS 8
#define INS_CLEARS 9

#define INS_ADD 10
#define INS_SUB 11
#define INS_MUL 12
#define INS_DIV 13
#define INS_IDIV 14

#define INS_ADDS 15
#define INS_SUBS 16
#define INS_MULS 17
#define INS_DIVS 18
#define INS_IDIVS 19

#define INS_LT 20
#define INS_GT 21
#define INS_EQ 22

#define INS_LTS 23
#define INS_GTS 24
#define INS_EQS 25

#define INS_AND 26
#define INS_OR 27
#define INS_NOT 28

#define INS_ANDS 29
#define INS_ORS 30
#define INS_NOTS 31

#define INS_INT2FLOAT 32
#define INS_FLOAT2INT 33
#define INS_INT2CHAR 34
#define INS_STRI2INT 35

#define INS_INT2FLOATS 36
#define INS_FLOAT2INTS 37
#define INS_INT2CHARS 38
#define INS_STRI2INTS 39

#define INS_READ 40
#define INS_WRITE 41

#define INS_CONCAT 42
#define INS_STRLEN 43
#define INS_GETCHAR 44
#define INS_SETCHAR 45

#define INS_TYPE 46

#define INS_LABEL 47
#define INS_JUMP 48
#define INS_JUMPIFEQ 49
#define INS_JUMPIFNEQ 50
#define INS_JUMPIFEQS 51
#define INS_JUMPIFNEQS 52

#define INS_EXIT 53

#define INS_BREAK 54
#define INS_DPRINT 55

//definovani vlastnich instrukci
#define PI_BEGINFUNC 60
#define PI_ENDFUNC 61

#define PI_INIT 62
#define PI_LTE 63
#define PI_GTE 64
#define PI_NEQ 65
#define PI_ADDSTR 66 //secte dva strinky
#define PI_ADD 67	//secte dve cisla
#define PI_ASS 81 //expression je hotovy, ulozit promennou
#define PI_ASS_FUNC 82
#define PI_ASS_DECL 83
#define PI_ASS_DECL_FUNC 84
#define PI_DEF_PARAM 85
#define PI_FCALL 86
#define PI_FCALL_PARAMT 87
#define PI_FCALL_PARAMID 88
#define PI_IF_START 89
#define PI_IF_ELSE 90
#define PI_IF_END 91
#define PI_WHILE_START 92
#define PI_WHILE_END 93

typedef struct{
	int ins_type;
	void *adr1;
	void *adr2;
	void *adr3;
}t_Ins;

typedef struct element{
	t_Ins *data;
	struct element *next;
	struct element *prev;
}t_Elem;

typedef struct{
	t_Elem *first;
	t_Elem *act;
	t_Elem *last;
}t_InsList;


t_InsList *list; //globalni promenna na list obsahujici instrukce

int listInit();
int addInst(int ins_type, void *adr1, void *adr2, void *adr3, int act);
void setActive(t_Elem *elem);
void rewriteActive(int ins_type, void *adr1, void *adr2, void *adr3);
void deleteFirst();
void freeList();
void printList();

#endif //INSLIST_H
