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

//definice lokalni funkci
void debug_print(struct table *local_table, t_Token t, t_Token tb);
int resultType(int t1, int t2);

int checkRule(t_IStack *s, int *type){
    int elem1, elem2, elem3;
	int type1, type2, type3;

    elem1 = i_topPop(s, &type1);
    if (elem1 == T_ID || elem1 == T_DOUBLE || elem1 == T_INT || elem1 == T_STRING)
	{
		*type = type1;
		return R_ID;
	}

	i_display(s);
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
				addInst(INS_ADD, NULL, NULL, NULL, 0);
                return R_PLUS;
            case T_MINUS:
				addInst(INS_SUB, NULL, NULL, NULL, 0);
                return R_MINUS;
            case T_MUL:
				addInst(INS_MUL, NULL, NULL, NULL, 0);
                return R_MUL;
            case T_DIV:
				addInst(INS_DIV, NULL, NULL, NULL, 0);
				return R_DIV;
            case T_LESS:
				addInst(INS_LT, NULL, NULL, NULL, 0);
                return R_LESS;
            case T_MORE:
				addInst(INS_GT, NULL, NULL, NULL, 0);
                return R_MORE;
            case T_LESS_EQ:
				addInst(PI_LTE, NULL, NULL, NULL, 0);
                return R_LESSEQ;
            case T_MORE_EQ:
				addInst(PI_GTE, NULL, NULL, NULL, 0);
                return R_MOREEQ;
            case T_EQ_REL:
				addInst(INS_EQ, NULL, NULL, NULL, 0);
                return R_EQ;
            case T_NOT_EQ:
				addInst(PI_NEQ, NULL, NULL, NULL, 0);
                return R_NEQ;
            default:
                fprintf(stderr, "ERROR SYNTAX\n");
                return ERROR_SYNTAX;
        }
    }
    else
    {
        fprintf(stderr, "ERROR SYNTAX\n");
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
		case T_NIL: //TODO
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



t_Token exprParse(t_Token t, t_Token tb, struct table *local_table, int usingTb){
    /* Precedencni tabulka */
	debug_print(local_table, t, tb); //DEBUG PRINT TODO

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

    /* Algoritmus z prednasky*/
    do {
        //terminal z vrcholu
        int a = i_termTop(&s, &type);

        b = b_token.type;
        switch (prec_table[tokenToIndex(a)][tokenToIndex(b)])
        {
            case PT_E:
				//fprintf(stderr, "PT_E\n");

				addInitInstruction(&s, local_table, b_token);

				//test jestli mame nacteny token
				if (usingTb == 1)
				{
                    b_token = tb;
                    usingTb = 0;
                }
				else
				{
                    b_token = getNextToken(&error);
					printToken(b_token, 0);
                }
                if (error == ERROR_LEX) exit(ERROR_LEX);
                break;

            case PT_L:
                //fprintf(stderr, "PT_L\n");

				i_termTopPush(&s, PT_L, NON_TYPE);
				addInitInstruction(&s, local_table, b_token);

				if (usingTb == 1)
				{
                    b_token = tb;
                    usingTb = 0;
                }
				else
				{
                    b_token = getNextToken(&error);
					printToken(b_token, 0);
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
					fprintf(stderr, "EXPR ERROR: spatny symbol na zasobniku");
                    exit(ERROR_SYNTAX);
                }
                break;
            default:
				fprintf(stderr, "EXPR ERROR: prazdne misto v tabulce");
                exit(ERROR_SYNTAX);
        }
    } while(!isEnd(b) || !isEnd(i_termTop(&s, &type)));
    i_stackDestroy(&s);
	return b_token;
}

//vrati hodnotu, jestli dany symbol je ukoncujici symbol $
int isEnd(int val)
{
    return (val == T_DO || val == T_THEN || val == T_EOL || val == PT_END) ? 1 : 0;
}

//vrati vysledny typ
int resultType(int t1, int t2)
{
	if (t1 == t2)
		return t1;
	else if (t1 == T_INT || t2 == T_DOUBLE)
		return T_DOUBLE;
	else if (t2 == T_INT || t1 == T_DOUBLE)
		return T_DOUBLE;
	else
	{
			fprintf(stderr, "ERROR_SEM_COMPATIBILITY\n");
			return NON_TYPE;
			//exit(ERROR_SEM_COMPATIBILITY);
	}
}

//prida instrukci do listu, ktera definuje novou promennou
void addInitInstruction(t_IStack *s, struct table *local_table, t_Token b_token)
{
	t_Node *aux;
	int b = b_token.type;
	int found = 0; //indikator jestli se idendifikator tokenu nasel v lokalni tabulce symbolu

	if (b == T_ID)
	{
		 //hledej v lokalni tabulce
		if (local_table != NULL)		
		{
			aux = tableSearchItem(local_table, b_token.attr);
			if (aux != NULL) //nasli jsme
				found = 1;
		}

		//pokud jsme nenasli, hledej jeste v globalni tabulce symbolu
		if (!found) //
		{	
			aux = tableSearchItem(&table, b_token.attr);
			if (aux == NULL) 			//nenasli jsme ani v globalni tabulce symbolu
			{
				fprintf(stderr, "EXPR ERROR: nedefinovana promenna ve vyrazu\n");
				exit(ERROR_SEMANTIC);	// ---> chyba: prace s nedefinovanou promennou
			}
		}

		//funkce ve vyrazu nesmi byt
		if (aux->data->is_var == 0)
		{
			fprintf(stderr, "EXPR ERROR: funkce ve vyrazu\n");
			exit(ERROR_SEM_COMPATIBILITY);
		}

		i_push(s, b, b);
		addInst(PI_INIT, NULL, (void*) aux->data, NULL, 0); //TODO
	}
    else if (b == T_INT)
	{
		i_push(s, b, b);
		addInst(PI_INIT, NULL, (void*)b_token.attr.val, (void*)T_INT, 0);
	}
	else if (b == T_DOUBLE)
	{
		i_push(s, b, b);
		addInst(PI_INIT, NULL, (void*)b_token.attr.val, (void*)T_DOUBLE, 0);
	}
	else if (b == T_STRING)
	{
		i_push(s, b, b);
		addInst(PI_INIT, NULL, (void*)b_token.attr.val, (void*)T_STRING, 0);
	}
	else if (b == T_NIL) //TODO
	{
		i_push(s, b, b);
		addInst(PI_INIT, NULL, (void*)b_token.attr.val, (void*)T_NIL, 0);
	}
	else
		i_push(s, b, NON_TYPE);
}

//vypise jestli jsem zanoreni ve funkci nebo ne
void debug_print(struct table *local_table, t_Token t, t_Token tb)
{
    if (local_table == NULL)
	{
        fprintf(stderr, "--globalni scope\n");
    }
	else
	{
        fprintf(stderr, "--lokalni scope\n");
        tablePrint(local_table, 1);
    }
	fprintf(stderr, "**** EXPR ****\n");
	printToken(t, 0); 
	printToken(tb, 0);
}
