/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 * 	Hlavickovy soubor pro ADT dvojsmerne vazany seznam 
*/

#ifndef INSLIST_H
#define INSLIST_H

#include "ret_vals.h"
#include "symtable.h"
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

/*----------------------------------*/
/*	definovani vlastnich instrukci	*/
/*----------------------------------*/

#define PI_BEGINFUNC 60	//zacatek funkce
#define PI_ENDFUNC 61	//konec funkce

#define PI_INIT 62 //inicializace promente
#define PI_LTE 63 // instrokuce pro vykonani <=
#define PI_GTE 64 // instrokuce pro vykonani >=
#define PI_NEQ 65	// instrokuce pro vykonani !=
#define PI_ADDSTR 66 //secte dva strinky
#define PI_ADD 67	//secte dve cisla
#define PI_ASS 81 //expression je hotovy, ulozit promennou
#define PI_ASS_FUNC 82	//prirazeni navratove hodnoty
#define PI_ASS_DECL 83	//deklarace + prirazeni vyrazu/promenne
#define PI_ASS_DECL_FUNC 84 //deklarace + prirazeni navr hodnoty funkce
#define PI_DEF_PARAM 85	//parametr pri definici
#define PI_FCALL 86 //volani funkce
#define PI_FCALL_PARAMT 87 //parametr, ktery neni id
#define PI_FCALL_PARAMID 88 //parametr promenna
#define PI_IF_START 89 //zacatek ifu
#define PI_IF_ELSE 90 //else
#define PI_IF_END 91 //konec ifu
#define PI_WHILE_START 92 //zacatek while navesti
#define PI_WHILE_EX 93 //vyraz v ifu
#define PI_WHILE_END 94 //konec ifu
#define PI_WHILE_DECL 96	//deklarace promenne vyhozena pred while
#define PI_WHILE_DECL_FUNC 96 //deklarace funkce vyhozena pred while
#define PI_BUILTFUNC 95	//vestavene funkce

typedef struct{
	int ins_type; //typ instrukce
	void *adr1; //pomocna adresa1
	void *adr2; //pomocna adresa2
	void *adr3; //pomocna adresa3
}t_Ins;

typedef struct element{
	t_Ins *data; //uzitacne data elementu
	struct element *next; //ukazatel na nasledujici prvek
	struct element *prev; //ukazatel na predchozi prvek
}t_Elem;

typedef struct{
	t_Elem *first; //ukuzatel na prvni element v listu
	t_Elem *act; //ukazatel na aktivni prvek v listu
	t_Elem *last; //ukazatel na posledni prvek v listu
}t_InsList;

t_InsList *list; //globalni promenna na list obsahujici instrukce

/*
inicializuje list
@return int MEMORY_OK v uspechu, jinak MEMORY_ERROR
*/
int listInit();

/*
prida instrukci do listu
@param int typ instrukce
@param void pomocna adresa1
@param void pomocna adresa2
@param void pomocna adresa3
@return int MEMORY_OK v uspechu, jinak MEMORY_ERROR
*/
int addInst(int ins_type, void *adr1, void *adr2, void *adr3, int act);

/*
nastavi aktivni prvek
@param t_Elem tento element se stane aktivni
*/
void setActive(t_Elem *elem);

/*
@param void typ instrukce
@param void pomocna adresa1
@param void pomocna adresa2
@param void pomocna adresa3
*/

void rewriteActive(int ins_type, void *adr1, void *adr2, void *adr3);
/*
vymaze prvni element v listu
*/
void deleteFirst();

/*
vlozi instrukci na konec listu
@param t_Ins insturkce ktere se vlozi 
@return int MEMORY_OK v uspechu, jinak MEMORY_ERROR
*/
int insertLast(t_Ins *ins);

/*
vlozi instrukci pred aktivni prvek
@param t_Ins insturkce ktere se vlozi 
@return int MEMORY_OK v uspechu, jinak MEMORY_ERROR
*/
int insertBeforeActive(t_Ins *ins);

/*----------------------*/
/*	debugovaci funkce 	*/
/*----------------------*/
void freeList();
void printList();

#endif //INSLIST_H
