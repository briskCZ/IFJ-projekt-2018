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

int resultType(int t1, int t2)
{
	if (t1 == t2)
		return t1;
	else if ((t1 == T_INT || t1 == T_DOUBLE) && (t2 == T_INT || t2 == T_DOUBLE))
	{
		/*
		DEFVAR v1
		INT2FLOAT v1, 
		
		*/

		return T_DOUBLE;
	}
	else 
	{
			fprintf(stderr, "ERROR_SEM_COMPATIBILITY\n");
			exit(ERROR_SEM_COMPATIBILITY);
	}
}

int checkRule(t_IStack *s, int *type){
    int elem1, elem2, elem3;
	int type1, type2, type3;
	
    elem1 = i_topPop(s, &type1);
    if (elem1 == T_ID || elem1 == T_DOUBLE || elem1 == T_INT || elem1 == T_STRING)
	{
		*type = type1;
		return R_ID;
	}
	
    elem2 = i_topPop(s, &type2);
    elem3 = i_topPop(s, &type3);
    if (elem1 == T_RIGHT_PAR && elem2 == PT_E_RULE && elem3 == T_LEFT_PAR)
	{
		*type = type2;
		return R_PAR;
	}
    else if (elem1 == PT_E_RULE && elem3 == PT_E_RULE)
    {
		//konverze typu
		*type = resultType(type1, type3);

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

t_Token exprParse(t_Token t, t_Token tb, int usingTb){
    /* Precedencni tabulka */
    int prec_table[PT_SIZE][PT_SIZE] = {
        {PT_R, PT_L, PT_R, PT_L, PT_R, PT_L, PT_R},
        {PT_R, PT_R, PT_R, PT_L, PT_R, PT_L, PT_R},
        {PT_L, PT_L, PT_X, PT_L, PT_R, PT_L, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_E, PT_L, PT_X},
        {PT_R, PT_R, PT_R, PT_X, PT_R, PT_X, PT_R},
        {PT_R, PT_R, PT_R, PT_X, PT_R, PT_X, PT_R},
        {PT_L, PT_L, PT_L, PT_L, PT_L, PT_L, PT_X}
    };
    int error, r, b, type, temp;
    t_IStack s = i_stackInit();
    //vlozeni koncoveho symbolu na zasobnik
    i_push(&s, PT_END, NON_TYPE);
    //aktualni token na vstupu
    t_Token b_token = t;

    if (error == ERROR_LEX) exit(ERROR_LEX);
    /* Algoritmus z prednasky*/
    do {
        //terminal z vrcholu
        int a = i_termTop(&s, &type);

        b = b_token.type;
        //fprintf(stderr, "[%d, %d] || [%d, %d]\n", a, b, tokenToIndex(a), tokenToIndex(b));
        switch (prec_table[tokenToIndex(a)][tokenToIndex(b)])
        {
            case PT_E:
            //fprintf(stderr, "PT_E\n");

                i_push(&s, b, b); //u T_ID nezname typ, musime se podivat do tabulky symbolu
                if (usingTb == 1){
                    b_token = tb;
                    usingTb = 0;
                }else{
                    b_token = getNextToken(&error);
                    //CHECK_ERROR(error);

                }
                if (error == ERROR_LEX) exit(ERROR_LEX);
                break;

            case PT_L:
                //fprintf(stderr, "PT_L\n");
                //fprintf(stderr, "b: "); i_display(&s);

                i_termTopPush(&s, PT_L, NON_TYPE);
                //fprintf(stderr, "a: "); i_display(&s);
                i_push(&s, b, b); //TODO
                if (usingTb == 1){
                    b_token = tb;
                    usingTb = 0;
                }else{
                    b_token = getNextToken(&error);
                    //CHECK_ERROR(error);
                }
                if (error == ERROR_LEX) exit(ERROR_LEX);
                break;

            case PT_R:
                //fprintf(stderr, "PT_R\n");

                r = checkRule(&s, &type);
                if (r != ERROR_SYNTAX)
                {
                    if (i_topPop(&s, &temp) == PT_L)
                    {
                        i_push(&s, PT_E_RULE, type);
                        fprintf(stderr, "expr_parser: RULE: %d | %d\n", r, type);
                    }
                }
                else
                {

                    //fprintf(stderr, "EXPR: SYN1 ERROR\n");

                    exit(ERROR_SYNTAX);
                }
                break;
            default:

                //fprintf(stderr, "EXPR: SYN2 ERROR\n");
                exit(ERROR_SYNTAX);
        }
        //i_display(&s);
        //fprintf(stderr,"i_termTop: %d\n", i_termTop(&s, &temp));
        //fprintf(stderr,"DEBUG: ISENDB: %d | ISENDST: %d\n", isEnd(b), isEnd(i_termTop(&s, &temp)));
    } while(!isEnd(b) || !isEnd(i_termTop(&s, &type)));
    i_stackDestroy(&s);
	return b_token;
}



int main(){

}

