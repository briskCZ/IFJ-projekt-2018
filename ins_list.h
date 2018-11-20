#ifndef INSLIST_H
#define INSLIST_H 

#include "ret_vals.h" 
#include <stdlib.h>
#include <stdio.h>

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
