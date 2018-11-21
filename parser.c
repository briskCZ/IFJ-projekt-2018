/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Implementace parseru - rekurzivni sestup
*/
#include "parser.h"

#define CHECK_ERROR(err) if (err == ERROR_INTERNAL) {fprintf(stderr, "INTERNAL ERROR\n"); exit(ERROR_INTERNAL);}\
                         if (err == ERROR_LEX) {fprintf(stderr, "LEXICAL ERROR: on line: %d\n", sc_line_cnt); exit(ERROR_LEX);}

#define PRINT_SYNTAX_ERROR(symbol) {fprintf(stderr, "ERROR_SYNTAX: %s expected on line %d\n", symbol, sc_line_cnt);\
                                   exit(ERROR_SYNTAX);}

#define SAVE_ERROR(error){if (ERROR == 0) ERROR = error;}
#define P(a) fprintf(stderr, "%s\n", a);

//int ERROR = 0;

void assign(t_Token left, t_Token ass){
    P("--assign");
    t_Token ta, tb;
    int error = 0;
    ta = getNextToken(&error);
    CHECK_ERROR(error);
    /* ID = ID */
    if (ta.type == T_ID){
        tb = getNextToken(&error);
        CHECK_ERROR(error);
        /*  ID = ID ID  || ID = ID ( ID = ID "test" - fcall
        */
        if (tb.type == T_EOL){
            return;
        }
        switch (tb.type){
            case T_ID:
            case T_LEFT_PAR:
            case T_DOUBLE:
            case T_INT:
            case T_STRING:
                // volani funkce
                //fprintf(stderr, "L: %s, ASS: %s, R: %s, P1: %s\n", left.attr.val, ass.type, ta.attr.val, tb.attr);
                f_call(ta, tb);
                break;
            case T_PLUS:
            case T_MINUS:
            case T_MUL:
            case T_DIV:
            case T_LESS:
            case T_LESS_EQ:
            case T_MORE:
            case T_MORE_EQ:
            case T_EQ_REL:
            case T_NOT_EQ:
                /* expr */
                P("--expr v assign");
                returnToken(exprParse(ta, tb, 1));

                break;
            default:
            PRINT_SYNTAX_ERROR("Function call or expression");

        }
    }else if (ta.type == T_INT || ta.type == T_DOUBLE || ta.type == T_STRING){
        return;
    }
}
void f_call(t_Token ta, t_Token tb){
    //ta a tb jenom pro informaci o ID a dalsim tokenu nactenem po volani funkce
    P("--fcall");
    // fprintf(stderr, "--s: %s =\n", ta.attr.val);
    switch (tb.type){
        case T_LEFT_PAR:
            param1();
            break;
        case T_ID:
        case T_DOUBLE:
        case T_INT:
        case T_STRING:
            // uz je prvni parametr funkce, ukladat nekam !!!
            param2(tb);
            break;
        case T_EOL:
            return;
        default:
            PRINT_SYNTAX_ERROR("Function parameter");
    }

}
void param1(){
    P("--param1");
    int error = 0;
    P("--token v param1");
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if (token.type == T_RIGHT_PAR){
        return;
    }else{
        //term se postara aby se program ukoncil s chybou
        term(token);
		//
		if (node != NULL)
		{	
			printToken(token, 0);
			if (arrParamAdd(node->data, token.attr) == MEMORY_ERROR) exit(ERROR_INTERNAL);
			
			t_Node *temp = tableInsertToken(node->data->local_symTable, token);
			tableChangeItemByNode(temp, 1, 0, 0, 0);
		}
		//###########################################
        param11();
    }
}
void param11(){
	
    P("--param11");
    int error = 0;
    P("--token v param11 na zacatku");
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if (token.type == T_COMMA){
        P("--token po comma");
        token = getNextToken(&error);
        CHECK_ERROR(error);
        term(token);
        //dalsi mozny parametr funkce, check v tabulce symbolu
		//
		if (node != NULL)
		{	
			printToken(token, 0);
			if (arrParamAdd(node->data, token.attr) == MEMORY_ERROR) exit(ERROR_INTERNAL);

			t_Node *temp = tableInsertToken(node->data->local_symTable, token);
			tableChangeItemByNode(temp, 1, 0, 0, 0);
			}
		//###########################################
        param11();
    }else if (token.type == T_RIGHT_PAR){
        P("--token v param11 right par");
        token = getNextToken(&error);
        CHECK_ERROR(error);
        if (token.type == T_EOL){
            return;
        }else{
            PRINT_SYNTAX_ERROR("EOL after PARAM");
        }
    }else{
        PRINT_SYNTAX_ERROR("Parameter");
    }
}

void param2(t_Token token){
    P("--param2");
    // int error = 0;
    // t_Token token = getNextToken(&error);
    // CHECK_ERROR(error);
    if (token.type == T_EOL){
        return;
    }else{
        term(token);
        param22();
    }
}
void param22(){
    P("--param22");
    int error = 0;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if (token.type == T_COMMA){
        token = getNextToken(&error);
        CHECK_ERROR(error);
        term(token);
        param22();
    }else if (token.type == T_EOL){
        return;
    }else{
        PRINT_SYNTAX_ERROR("Parameter");
    }
}
void term(t_Token token){
    P("--term");
    switch (token.type){
        case T_ID:
        case T_INT:
        case T_DOUBLE:
        case T_STRING:
            return;
        default:
            PRINT_SYNTAX_ERROR("Terminal");
    }
}
void sec1(){
    P("--sec1");
    int error = 0;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if (token.type == T_EOF){
        PRINT_SYNTAX_ERROR("END");
    }else if (token.type == T_END){
        return;
    }else if (token.type != T_EOL){
        P("--nechapu");
        //printToken(token, error);
        code(token);
        sec1();
    }

}
void sec2(){
    P("--sec2");
    int error = 0;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if(token.type == T_ELSE){
        t_Token token = getNextToken(&error);
        CHECK_ERROR(error);
        if (token.type == T_EOL){
            sec1();
        }else{
            PRINT_SYNTAX_ERROR("EOL after ELSE");
        }
    //pokud token neni else
    }else if (token.type != T_EOF){
        code(token);
        sec2();
    }else{
        PRINT_SYNTAX_ERROR("ELSE");
    }
}
void code(t_Token token){
	t_Node *temp;
    P("--code");
    int error = 0;
    P("--potencionalni problem");
    printToken(token, error);
    switch (token.type){
        case T_IF:
            /* IF */
            P("--IF");
            token = getNextToken(&error);
            CHECK_ERROR(error);
            token = exprParse(token, token, 0); //TODO
            if (token.type == T_THEN){
                P("--then");
                token = getNextToken(&error);
                CHECK_ERROR(error);
                if (token.type == T_EOL){
                    sec2();
                    sec1();
                }else{
                    PRINT_SYNTAX_ERROR("EOL after THEN");
                }
            }else{
                PRINT_SYNTAX_ERROR("THEN");
            }
            break;
        case T_WHILE:
            P("--WHILE");
            token = getNextToken(&error);
            CHECK_ERROR(error);
            token = exprParse(token, token, 0);
            if (token.type == T_DO){
                token = getNextToken(&error);
                CHECK_ERROR(error);
                if (token.type == T_EOL){
                    sec1();
                }else{
                    PRINT_SYNTAX_ERROR("EOL after DO");
                }
            }else{
                PRINT_SYNTAX_ERROR("DO");
            }
            break;
        case T_LEFT_PAR:
        case T_INT:
        case T_DOUBLE:
        case T_STRING:
            returnToken(exprParse(token, token, 0));
            break;
        case T_ID:
            /* TODO dalsi prace s tabulkou symbolu*/
			temp = tableInsertToken(&table, token);
			
			//##################################
            {
            t_Token tb = getNextToken(&error);
            CHECK_ERROR(error);
            switch (tb.type){
                case T_ASSIGNMENT:
                /* ID = */
                //P("--predprdel");
                //printToken(token, 99);
                //printToken(token, 99);

				tableChangeItemByNode(temp, 1, 0, 0, 1); //TODO
				
                assign(token, tb);
                    break;
                /* ID -> volani funkce */
                case T_ID:
                case T_LEFT_PAR:
                case T_DOUBLE:
                case T_INT:
                case T_STRING:
                case T_EOL:
                    // volani funkce
					
					tableChangeItemByNode(temp, 0, 0, 0, 1); //TODO
					
                    f_call(token, tb);
                    break;
                /* random vyraz */
                case T_PLUS:
                case T_MINUS:
                case T_MUL:
                case T_DIV:
                case T_LESS:
                case T_LESS_EQ:
                case T_MORE:
                case T_MORE_EQ:
                case T_EQ_REL:
                case T_NOT_EQ:
                    /* expr */
                    returnToken(exprParse(token, tb, 1));
                    break;
                default:
                    PRINT_SYNTAX_ERROR("Function call, assignment or expression");
            }
            break;
        }
        //case T_EOL: P("--EOL") break;
        //default: PRINT_SYNTAX_ERROR("SPECIAL");
    }
}

void program(){
    P("--program");
    int error = 0;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    //TODO????
    switch (token.type){
        case T_EOF:
            return;
        case T_DEF:
            /* Definice funkce */
			
            P("--def");
            token = getNextToken(&error);
            CHECK_ERROR(error);
            if (token.type == T_ID){
                //TODO pridat do tabulky symbolu zaznam o definici funkce
								
				node = tableInsertToken(&table, token);
				if (node != NULL)
					tableChangeItemByNode(node, 0, 0, 1, 1);
				else
					exit(ERROR_INTERNAL); //todo
			
				//#################################################
                token = getNextToken(&error);
                CHECK_ERROR(error);
                if (token.type == T_LEFT_PAR){
                    param1();
                    sec1();
                    program();
                }else{
                    PRINT_SYNTAX_ERROR("(");
                }
            }else{
                PRINT_SYNTAX_ERROR("ID");;
            }
            break;
        default:
            code(token); //TODO check error
            program(); //TODO check error
            break;


    }
}

int main(){
    ungetc('\n', stdin);
 
	//inicializace potrebnych veci
	scannerInit();
	table = tableInit();
	listInit();

	
    program();
	
	P("=========== TABLE PRINT ===============");
	tablePrint(&table, 0);
	
	printList();
	
	//uvolneni zdroju
    scannerClean();
	tableDestroy(&table);
	freeList();
    return SUCCESS;
}
