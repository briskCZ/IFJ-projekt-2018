#include "trash.h"
#define MAX 100
char* trashArr[MAX];
int t_index = 0;

void trashStoreMem(void *val){
    trashArr[t_index] = val;
    t_index++;
}
void trashFreeAll(){
    for (int i = 0; i <= t_index; i++){
        free(trashArr[i]);
    }
}
// t_trashItem *first;
//
// void trashPrint(){
// }
// void trashStoreMem(void * val){
//     t_trashItem *new = malloc(sizeof(t_trashItem));
//     if (new == NULL) exit(ERROR_INTERNAL);
//     new->mem = val;
//     new->next = NULL;
//     if (first == NULL){
//         first = new;
//     }else{
//         t_trashItem *item = first;
//         while(item->next != NULL){
//             item = item->next;
//         }
//         item->next = new;
//     }
// }
// void trashFreeAll(){
//     while(first != NULL){
//         t_trashItem *itemToDel = first;
//         first = itemToDel->next;
//         if (itemToDel != NULL){
//             free(itemToDel->mem);
//             free(itemToDel);
//         }
//     }
// }
