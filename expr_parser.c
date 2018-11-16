/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Implementace precedencni analyzy vyrazu
*/

#include "expr_parser.h"

int isEnd(int val){
    return (val == DO || val == THEN || val == T_EOL) ? PT_END_INDEX : 0;
}

int exprParse(){
    int prec_table[PT_SIZE][PT_SIZE] = {
        {PT_R, PT_R, PT_L, PT_L, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_L, PT_R, PT_L, PT_R},
        {PT_R, PT_R, PT_L, PT_L, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_L, PT_R, PT_L, PT_R},
        {PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_L, PT_R, PT_L, PT_R},
        {PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_L, PT_R, PT_L, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_E, PT_L, PT_X},
        {PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_X, PT_R, PT_X, PT_R},
        {PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_X, PT_R, PT_X, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_X, PT_L, PT_L, PT_X}
    };
    t_IStack stack = i_stackInit();
    int error, a;
    t_Token b;
    i_push(&stack, PT_END_INDEX);

    do{
        a = i_topPop(&stack);
        b = getNextToken(&error);
        if (error == ERROR_LEX) return ERROR_LEX;
        switch (prec_table[a][b.type]){
            // =
            case PT_E:
                fprintf(stderr, "DEBUG: PT_E\n");
                i_push(&stack, b.type);
                b = getNextToken(&error);
                if (error == ERROR_LEX) return ERROR_LEX;
                break;
            // <
            case PT_L:
                fprintf(stderr, "DEBUG: PT_L\n");
                a = i_topPop(&stack);
                i_push(&stack, PT_L);
                i_push(&stack, a);
                i_push(&stack, b.type);
                b = getNextToken(&error);
                if (error == ERROR_LEX) return ERROR_LEX;
            break;
            case PT_R:
                fprintf(stderr, "DEBUG: PT_R jeste nevime\n");
            break;
            case PT_X:
                fprintf(stderr, "Error precedencni analyzy\n");
                return ERROR_SYNTAX;
            break;
        }

    }while(!isEnd(i_topPop(&stack)));
    i_stackDestroy(&stack);
}
int main(){

    scannerInit();
    exprParse();
    scannerClean();

}
