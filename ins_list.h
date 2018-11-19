#ifndef INSLIST_H
#define INSLIST_H 

#include "ret_vals.h" 

typedef struct{
	int ins_type;
	void *adr1;
	void *adr2;
	void *adr3;
}t_Ins;

typedef struct element{
	tIns *data;
	struct element *next;
	struct element *prev;
}t_Elem;

typedef struct{
	t_Elem; *first;
	t_Elem; *act;
	t_Elem; *last
}t_InsList;


t_InsList *list; //globalni promenna na list obsahujici instrukce

void listInit();
int addInst(int ins_type, void *adr1, void *adr2, void *adr3, int act);
void setActive(t_Elem *elem);
void rewriteActive(t_Ins *ins);
void freeList();


#endif //INSLIST_H 