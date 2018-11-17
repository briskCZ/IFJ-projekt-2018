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


int checkRule(t_IStack *s){
    int elem1, elem2, elem3;

    elem1 = i_topPop(s);
    if (elem1 == ID)
        return R_ID;

    elem2 = i_topPop(s);
    elem3 = i_topPop(s);
    if (elem1 == RIGHT_PAR && elem2 == PT_E_RULE && elem3 == LEFT_PAR)
        return R_PAR;
    else if (elem1 == PT_E_RULE && elem3 == PT_E_RULE)
    {
        switch (elem2)
        {
            case PLUS:
                return R_PLUS;
            case MINUS:
                return R_MINUS;
            case MUL:
                return R_MUL;
            case DIV:
                return R_DIV;
            case ID:
                return R_ID;
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
                fprintf(stderr, "ERROR1: checkRule\n");
                return ERROR_SYNTAX;

        }
    }
    else
    {
        fprintf(stderr, "ERROR2: checkRule\n");
        return ERROR_SYNTAX;
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
        {PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_X}
    };
    int error, r, b;

    t_IStack s = i_stackInit();
    i_push(&s, PT_END_INDEX);


    t_Token b_token = getNextToken(&error);
    if (error == ERROR_LEX) return ERROR_LEX;


    do {
        int a = i_termTop(&s);
        b = b_token.type;

        fprintf(stderr, "[%d, %d]\n", a, b);
        switch (prec_table[a][b])
        {
            case PT_E:
            fprintf(stderr, "PT_E\n");
                i_push(&s, b);
                b_token = getNextToken(&error);
                if (error == ERROR_LEX) return ERROR_LEX;
                break;

            case PT_L:
                fprintf(stderr, "PT_L\n");
                // int a = i_topPop(&s);
                // i_push(&s, PT_L);
                // i_push(&s, a);

                fprintf(stderr, "b: "); i_display(&s);
                i_termTopPush(&s, PT_L);
                fprintf(stderr, "a: "); i_display(&s);


                i_push(&s, b);
                b_token = getNextToken(&error);
                if (error == ERROR_LEX) return ERROR_LEX;
                break;

            case PT_R:
                fprintf(stderr, "PT_R\n");
                r = checkRule(&s);
                if (r)
                {
                    if (i_topPop(&s) == PT_L)
                    {
                        i_push(&s, PT_E_RULE);
                        fprintf(stderr, "------- R: %d\n", r);
                    }
                }
                else
                {
                    fprintf(stderr, "SYN1 ERROR\n");
                    return ERROR_SYNTAX;
                }
                break;
            default:
                fprintf(stderr, "SYN2 ERROR\n");
                return ERROR_SYNTAX;
        }
        i_display(&s);
        printf("i_termTop: %d\n", i_termTop(&s));
    } while(!isEnd(b) || !isEnd(i_termTop(&s)));


    i_stackDestroy(&s);
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
