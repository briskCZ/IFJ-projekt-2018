/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Implementace funkci pro string
*/
#include "string.h"
//Inicialization of string structure
int stringInit(string *s){
    s->val = malloc(STR_ALLOC_INC*sizeof(char));
    if (s->val == NULL){
        return MEMORY_ERROR;
    }
    s->val[0] = '\0';
    s->length = 0;
    s->allocated_size = STR_ALLOC_INC;
    return MEMORY_OK;
}

//Frees dynamic memory used by string
void stringFree(string *s){
    free(s->val);
}

//vrati znak na zacatek stringu s
int stringUngetChar(char c, string *s){
    if (s->length + 1 >= s->allocated_size){
        s->val = realloc(s->val, (s->allocated_size + STR_ALLOC_INC)*sizeof(char));
        if (s->val == NULL){
            return MEMORY_ERROR;
        }else{
            s->allocated_size += STR_ALLOC_INC;
        }
    }
    s->length++;
    s->val[s->length + 1] = '\0';
    for (int i = s->length; i > 0; i--){
        s->val[i] = s->val[i-1];
    }
    s->val[0] = c;
    return MEMORY_OK;
}
void stringRemoveChar(string* s){
    if (s->length - 1 >= 0){
        s->val[s->length] == '\0';
        s->length--;
    }
}
//Vlozeni char *str do stringu s, s tim ze v pripade nedostatku pameti ji rozsiri TODO Bugged
int stringInsert(string *s, char *str){
    int memory_blocks_needed = 0;   //pocet bloku, potrebnych pro ulozeni *str
    //pokud je string delsi, nez alokovana velikost, proved realokaci
    if ((int)strlen(str) >= s->allocated_size){
        //Zjisteni, kolik je potreba alokovat mista (bloku po STR_ALLOC_INC)
        float new_s_size = strlen(str) / (float) STR_ALLOC_INC;
        if (new_s_size - (int)new_s_size > 0){
            new_s_size += 1;
        }
        memory_blocks_needed = (int)new_s_size;
        s->val = realloc(s->val, (memory_blocks_needed * STR_ALLOC_INC)*sizeof(char));
        if (s->val == NULL){
            return MEMORY_ERROR;
        }else{
            s->allocated_size = memory_blocks_needed * STR_ALLOC_INC;
            s->length = strlen(str);
            strcpy(s->val, str);
            return MEMORY_OK;
        }
    }else{
        s->length = strlen(str);
        strcpy(s->val, str);
        return MEMORY_OK;
    }
}
int stringCopy(string *sa, string *sb){
    if (sb->allocated_size > sa->allocated_size){
        sa->val = realloc(sa->val, sb->allocated_size * sizeof(char));
        if (sa->val == NULL){
            return MEMORY_ERROR;
        }
        sa->allocated_size = sb->allocated_size;
    }
    strcpy(sa->val, sb->val);
    sa->length = sb->length;
    return MEMORY_OK;
}

void stringClear(string *s){
    s->val[0] = '\0';
    s->length = 0;
}

int stringAddChar(string *s, char c){
    if (s->length + 1 >= s->allocated_size){
        s->val = realloc(s->val, s->allocated_size + STR_ALLOC_INC);
        if (s->val == NULL){
            return MEMORY_ERROR;
        }else{
            s->allocated_size += STR_ALLOC_INC;
        }
    }
    s->val[s->length] = c;
    s->val[++s->length] = '\0';
    return MEMORY_OK;
}

int stringCompare(string *sa, string *sb){
    return strcmp(sa->val, sb->val);
}

int stringCompareConst(string *sa, char *sb){
    return strcmp(stringGet(sa), sb);
}

int stringGetLength(string *s){
    return s->length;
}

char *stringGet(string *s){
    return s->val;
}

void stringPrint(string *s){
    fprintf(stderr, "%s | len: %d | alloc: %d\n", s->val, s->length, s->allocated_size);
}
/*
int main(){
    string x;
    stringInit(&x);
    // stringPrint(&x);
    // stringInsert(&x, "ahojkap");
    // stringPrint(&x);
    // printf("ret: %d\n", stringInsert(&x, "ahojKapuahojKapuahojKapu"));
    // stringPrint(&x);
    // for (int i = 0; i < 2; i++){
    //     printf("%d\n", i);
    //     stringAddChar(&x, 'a');
    // }
    // //stringClear(&x);
    // stringPrint(&x);
    // stringInsert(&x, "fungujes");
    // stringPrint(&x);

    stringPrint(&x);
    for (int i = 0; i < 15; i++){
        stringUngetChar(&x, 'f');
    }
    stringPrint(&x);
    stringFree(&x);
}*/
