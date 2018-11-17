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
    return (val == DO || val == THEN || val == T_EOL || val == T_EOF) ? PT_END_INDEX : 0;
}
int checkRule(t_IStack stack){
    int elem1, elem2, elem3;
    elem1 = i_topPop(&stack);
    printf("DEBUG: TU: |e1 %d\n", elem1);
    if (elem1 == ID){
        return R_ID;
    }else{
        elem2 = i_topPop(&stack);
        elem3 = i_topPop(&stack);
        if (elem1 == RIGHT_PAR && elem2 == PT_E_RULE && elem3 == LEFT_PAR){
            return R_PAR;
        }else if (elem1 == PT_E_RULE && elem3 == PT_E_RULE){
            printf("DEBUG: TU: |e1 %d |e2 %d |e3 %d\n", elem1, elem2, elem3);
            switch (elem2){
                case PLUS:
                    return R_PLUS;
                case MINUS:
                    return R_MINUS;
                case MUL:
                    return R_MUL;
                case DIV:
                    return R_DIV;
                case LESS:
                    return R_LESS;
                case MORE:
                    return R_MORE;
                case LESS_EQ:
                    return R_LESSEQ;
                case MORE_EQ:
                    return R_MOREEQ;
                case EQ_REL:
                    return R_EQ;
                case NOT_EQ:
                    return R_NEQ;
                default:
                    fprintf(stderr, "Syntactic error 1\n");
                    return ERROR_SYNTAX;
            }
        }else{
            fprintf(stderr, "Syntactic error 2\n");
            return ERROR_SYNTAX;
        }
    }
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
    t_Token b = getNextToken(&error);
    if (error == ERROR_LEX) return ERROR_LEX;
    i_push(&stack, PT_END_INDEX);

    do{
        a = i_termTop(&stack);
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
                i_push(&stack, PT_L);
                i_push(&stack, b.type);
                i_display(&stack);
                b = getNextToken(&error);
                if (error == ERROR_LEX) return ERROR_LEX;
            break;
            case PT_R:
            {
                int rule = checkRule(stack);
                printf(" DEBUG: RULE: %d\n", rule);
                if (rule == ERROR_SYNTAX || i_topPop(&stack) != PT_L) return ERROR_SYNTAX;
                i_push(&stack, PT_E_RULE);
            }
            break;
            case PT_X:
                fprintf(stderr, "Error precedencni analyzy\n");
                return ERROR_SYNTAX;
            break;
        }

        i_display(&stack);
        printf("B: %d\n", isEnd(b.type));
    }while(!(isEnd(i_termTop(&stack)) && isEnd(b.type)));
    i_stackDestroy(&stack);
}
int main(){

    /*scannerInit();
    t_IStack s = i_stackInit();
    t_Token token;
    do {
        int error;
        token = getNextToken(&error);
        i_push(&s, token.type);
        printToken(token, error);
    } while (token.type != T_EOF);

    i_display(&s);
    scannerClean();
    i_stackDestroy(&s);
    */
    scannerInit();
    exprParse();
    scannerClean();
    /*
    t_IStack s = i_stackInit();
    for (int i = 0; i < 100; i++){
        i_push(&s, i);
    }
    i_display(&s);
    i_stackDestroy(&s);
    */
}
