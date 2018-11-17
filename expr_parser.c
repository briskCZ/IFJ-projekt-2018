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
    return (val == T_DO || val == T_THEN || val == T_EOL || val == PT_END) ? 1 : 0;
}


int checkRule(t_IStack *s){
    int elem1, elem2, elem3;

    elem1 = i_topPop(s);
    if (elem1 == T_ID)
        return R_ID;

    elem2 = i_topPop(s);
    elem3 = i_topPop(s);
    if (elem1 == T_RIGHT_PAR && elem2 == PT_E_RULE && elem3 == T_LEFT_PAR)
        return R_PAR;
    else if (elem1 == PT_E_RULE && elem3 == PT_E_RULE)
    {
        switch (elem2)
        {
            case T_PLUS:
                return R_PLUS;
            case T_MINUS:
                return R_MINUS;
            case T_MUL:
                return R_MUL;
            case T_DIV:
                return R_DIV;
            case T_ID:
                return R_ID;
            case T_LESS:
                return R_LESS;
            case T_MORE:
                return R_MORE;
            case T_LESS_EQ:
                return R_LESSEQ;
            case T_MORE_EQ:
                return R_MOREEQ;
            case T_EQ_REL:
                return R_EQ;
            case T_NOT_EQ:
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

int tokenToIndex(int type){
    switch (type){
        case T_PLUS:
        case T_MINUS:
            return 0;
        case T_MUL:
        case T_DIV:
            return 1;
        case T_LESS:
        case T_LESS_EQ:
        case T_MORE:
        case T_MORE_EQ:
        case T_EQ_REL:
        case T_NOT_EQ:
            return 2;
        case T_LEFT_PAR:
            return 3;
        case T_RIGHT_PAR:
            return 4;
        case T_STRING:
        case T_DOUBLE:
        case T_INT:
        case T_ID:
            return 5;
        case T_EOL:
        case PT_END:
        case T_DO:
        case T_THEN:
            return 6;
        default:
            return -1;
    }
}
int exprParse(){
    // int prec_table[PT_SIZE][PT_SIZE] = {
    //     {PT_R, PT_R, PT_L, PT_L, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_L, PT_R, PT_L, PT_R},
    //     {PT_R, PT_R, PT_L, PT_L, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_L, PT_R, PT_L, PT_R},
    //     {PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_L, PT_R, PT_L, PT_R},
    //     {PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_L, PT_R, PT_L, PT_R},
    //     {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
    //     {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
    //     {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
    //     {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
    //     {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
    //     {PT_L, PT_L, PT_L, PT_L, PT_X, PT_X, PT_X, PT_X, PT_X, PT_X, PT_L, PT_R, PT_L, PT_R},
    //     {PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_E, PT_L, PT_X},
    //     {PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_X, PT_R, PT_X, PT_R},
    //     {PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_R, PT_X, PT_R, PT_X, PT_R},
    //     {PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_X}
    // };
    int prec_table[PT_SIZE][PT_SIZE] = {
        {PT_R, PT_L, PT_R, PT_L, PT_R, PT_L, PT_R},
        {PT_R, PT_R, PT_R, PT_L, PT_R, PT_L, PT_R},
        {PT_L, PT_L, PT_X, PT_L, PT_R, PT_L, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_E, PT_L, PT_X},
        {PT_R, PT_R, PT_R, PT_X, PT_R, PT_X, PT_R},
        {PT_R, PT_R, PT_R, PT_X, PT_R, PT_X, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_X}
    };
    int error, r, b;

    t_IStack s = i_stackInit();
    i_push(&s, PT_END);


    t_Token b_token = getNextToken(&error);
    if (error == ERROR_LEX) return ERROR_LEX;


    do {
        int a = i_termTop(&s);
        b = b_token.type;

        fprintf(stderr, "[%d, %d] || [%d, %d]\n", a, b, tokenToIndex(a), tokenToIndex(b));
        switch (prec_table[tokenToIndex(a)][tokenToIndex(b)])
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
        fprintf(stderr,"i_termTop: %d\n", i_termTop(&s));
        fprintf(stderr,"DEBUG: ISENDB: %d | ISENDST: %d\n", isEnd(b), isEnd(i_termTop(&s)));
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
