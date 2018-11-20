#include <stdio.h>
#include <stdlib.h>
#include "ret_vals.h"

#ifndef TRASH_H
#define TRASH_H
#define MAX 100

typedef struct t_trashElem{
    void *mem;
    struct t_trashElem *next;
}t_trashItem;

void trashPrint();
void trashStoreMem(void * val);
void trashFreeAll();
#endif
