/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Dynamicke pole tokenu
 */


#include "token_array.h"


int tarrInit(t_TokenArr *ta){
    ta->array = malloc(sizeof(t_Token) * TARR_MALLOC_SIZE);
    if (ta->array == NULL) return MEMORY_ERROR;
    ta->size = TARR_MALLOC_SIZE;
    ta->max_index = 0;
    ta->index = 0;
    return MEMORY_OK;
}

int tarrFill(t_TokenArr *ta){
    t_Token token;
    int error = 0;
    do {
        token = getNextToken(&error);
        if (error == ERROR_LEX || error == ERROR_INTERNAL){
            cleanAll();
            exit(error);
        }else{
            tarrAddToken(ta, token);
        }
    }while(token.type != T_EOF);
    sc_line_cnt = 1;
    return SUCCESS;
}

int tarrAddToken(t_TokenArr *ta, t_Token token)
{
    if(ta == NULL) return MEMORY_ERROR;
    if (ta->max_index + 1 > ta->size){
        ta->array = realloc(ta->array, sizeof(t_Token) * (ta->size + TARR_MALLOC_SIZE));
        if (ta->array == NULL) return MEMORY_ERROR;
        ta->size += TARR_MALLOC_SIZE;
    }
    ta->array[ta->max_index].type = token.type;
    stringInit(&(ta->array[ta->max_index].attr));
    stringCopy(&(ta->array[ta->max_index].attr), &token.attr);
    ta->max_index++;
    return MEMORY_OK;
}

t_Token tarrGetNextToken(t_TokenArr *ta){
    if (ta->index < ta->size){
        if (ta->array[ta->index].type == T_EOL) sc_line_cnt++;
        return ta->array[ta->index++];
    }
}

t_Token tarrGetPrintNextToken(t_TokenArr *ta){
    t_Token token = tarrGetNextToken(ta);
    printToken(token, 0);
    return token;
}
/*
Funkce zjisti informace o definovanych funkcich (jmeno + pocet parametru)
a vlozi je do tabulky symbolu jako nedefinovane, ale was_found
*/
void tarrGetFuncInfo(t_TokenArr *ta){
    for (int i = 0; i < ta->max_index; i++){
        if (ta->array[i].type == T_DEF){
            //DEF ID
            if (i + 1 < ta->max_index && ta->array[i+1].type == T_ID){
                t_Node *fun = tableInsertToken(&table, ta->array[i+1]);
                tableChangeItemByNode(fun, 0, 0, 0, 1);
                fun->data->was_found = 1;
                //DEF ID PARAM
                if (i + 2 < ta->max_index && ta->array[i+2].type == T_LEFT_PAR){
                    int j = i + 3;
                    int params_cnt = 0;
                    while(j < ta->max_index && ta->array[j].type == T_ID){
                        /* dalsi parametry */
                        if(++j < ta->max_index && (ta->array[j].type == T_COMMA || ta->array[j].type == T_RIGHT_PAR)){
                            j++;
                            params_cnt++;
                        }
                    }
                    fun->data->params_cnt = params_cnt;
                    i = j;
                }
            }
        }
    }
}

int tarrReturnToken(t_TokenArr *ta){
    ta->index--;
    if (ta->index < 0) return MEMORY_ERROR;
    return MEMORY_OK;
}

void tarrPrint(t_TokenArr *ta){
    for (int i = 0; i < ta->max_index; i++){
        printToken(ta->array[i], 0);
    }
}

void tarrFree(t_TokenArr *ta){
    for(int i = 0; i < ta->max_index; i++){
        stringFree(&(ta->array[i].attr));
    }
    free(ta->array);
}
