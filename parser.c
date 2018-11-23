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

//#define P(a) fprintf(stderr, "%s\n", a);
void assNew(int isNew, t_Node* leftVar, char* err);
//int ERROR = 0;
int isGlobal(){
    return (pa_funcLocalTable == NULL) ? 1 : 0;
}
void assNew(int isNew, t_Node* leftVar, char* err){
    if (isNew){
        tableChangeItemByNode(leftVar, 1, 0, 1, isGlobal());
    }
    if(leftVar->data->is_var == 0){
        fprintf(stderr, "ERROR_SEMANTIC: Cannot assign to function %s, on line: %d\n", err, sc_line_cnt);
        exit(ERROR_SEMANTIC);
    }
}
void assign(t_Token left, t_Token ass){
    //otestovat levou stranu v tabulce symbolu, dale pokud je funkce
    P("--assign");
    t_symTable *scopeTable;
    t_Node *leftVar;
    int isNew = 0, ret_type = 0;
    if (isGlobal()){
        scopeTable = &table;
    }else{
        scopeTable = pa_funcLocalTable;
    }
    //vyhledani leve strany v tabulce symbolu
    t_Node *n = tableSearchItem(scopeTable, left.attr);
    if (n == NULL){
        //polozka nebyla nalezena, definuje se nova promena
        leftVar = tableInsertToken(scopeTable, left);
        isNew = 1;
        //TODO kdyz bude funkce definovana potom nemusi byt nula
        tableChangeItemByNode(leftVar, -1, 0, 0, isGlobal());
    }else{
        leftVar = n;
    }
    t_Token ta, tb;
    int error = 0;
    ta = getNextToken(&error);
    CHECK_ERROR(error);
    /* ID = ID */
    if (ta.type == T_ID){
        tb = getNextToken(&error);
        CHECK_ERROR(error);
        if (tb.type == T_EOL){
            //pokud prirazujeme id do id
            t_Node *rightVar = tableSearchItem(scopeTable, ta.attr);
            if (rightVar == NULL){
                fprintf(stderr, "ERROR_SEMANTIC: Variable not defined: %s on line %d\n", stringGet(&ta.attr), sc_line_cnt);
                exit(ERROR_SEMANTIC);
            }else{
                if (rightVar->data->is_var == 0){
                    P("--assign fcall bez parametru");
                    //volani funkce bez parametru
                    f_call(ta, tb);
                    assNew(isNew, leftVar, stringGet(&left.attr));
                    //TODO BACHA MUZE TU BYT KRAVINA
                    tableChangeItemByNode(leftVar, 1, -1, 1, isGlobal());
                    if(isNew){
                        addInst(PI_ASS_DECL_FUNC, (void*)leftVar, (void*)rightVar, NULL, 0);
                    }else{
                        addInst(PI_ASS_FUNC, (void*)leftVar, (void*)rightVar, NULL, 0);
                    }
                    tablePrintItem(leftVar);
                    return;
                }else{
                    P("--assign jedne promenne");
                    //prirazeni
                    assNew(isNew, leftVar, stringGet(&left.attr));
                    tableChangeItemByNode(leftVar, 1, ret_type, 1, isGlobal());
                    if(isNew){
                        addInst(PI_ASS_DECL, (void*)leftVar, (void*)rightVar, NULL, 0);
                    }else{
                        addInst(PI_ASS, (void*)leftVar, (void*)rightVar, NULL, 0);
                    }
                    return;
                }
            }
        }
        /*  ID = ID ID  || ID = ID ( ID = ID "test" - fcall
        */
        switch (tb.type){
            case T_ID:
            case T_LEFT_PAR:
            case T_DOUBLE:
            case T_INT:
            case T_STRING:{
                /* Volani funkce v assign s parametry */
                //TODO POKUD JE PRINT SPECIALNI S X PARAMETRAMA
                t_Node *rightVar = tableSearchItem(scopeTable, ta.attr);
                if (rightVar == NULL){
                    fprintf(stderr, "ERROR_SEMANTIC: Variable not defined: %s on line %d\n", stringGet(&ta.attr), sc_line_cnt);
                    exit(ERROR_SEMANTIC);
                }else{
                    if (rightVar->data->is_var == 0){
                        P("--assign fcall s parametry");
                        //volani funkce bez parametru
                        //ablePrintItem(rightVar);
                        f_call(ta, tb);
                        tableChangeItemByNode(leftVar, 1, -1, 1, isGlobal());
                        assNew(isNew, leftVar, stringGet(&left.attr));
                        if(isNew){
                            addInst(PI_ASS_DECL_FUNC, (void*)leftVar, (void*)rightVar, NULL, 0);
                        }else{
                            addInst(PI_ASS_FUNC, (void*)leftVar, (void*)rightVar, NULL, 0);
                        }
                        return;
                    }else{
                        fprintf(stderr, "ERROR_SEMANTIC: %s is var, not function, line %d\n",stringGet(rightVar->data->name), sc_line_cnt);
                        exit(ERROR_SEMANTIC);
                    }
                }
                break;
            }
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

                returnToken(exprParse(ta, tb, pa_funcLocalTable, 1, &ret_type));
                assNew(isNew, leftVar, stringGet(&left.attr));
                tableChangeItemByNode(leftVar, 1, ret_type, 1, isGlobal());
                //pokud je novy zaznam v tabulce symbolu
                if (isNew){
                    addInst(PI_ASS_DECL, (void*)leftVar, NULL, NULL, 0);
                }else{
                    addInst(PI_ASS, (void*)leftVar, NULL, NULL, 0);
                }
                break;
            default:
            PRINT_SYNTAX_ERROR("Function call or expression");

        }
    }else if (ta.type == T_INT || ta.type == T_DOUBLE || ta.type == T_STRING || ta.type == T_NIL){

        P("--assign term");
        assNew(isNew, leftVar, stringGet(&left.attr));
        returnToken(exprParse(ta, ta, scopeTable, 0, &ret_type));
        tableChangeItemByNode(leftVar, 1, ret_type, 1, isGlobal());
        if (isNew){
            addInst(PI_ASS_DECL, (void*)leftVar, NULL, NULL, 0);
        }else{
            addInst(PI_ASS, (void*)leftVar, NULL, NULL, 0);
        }

        //vygeneruj piass (a, ta.attr, typ, 0);
    }else{
        PRINT_SYNTAX_ERROR("in assign: fcall or exp");
    }
}
void f_call(t_Token ta, t_Token tb){
    //ta a tb jenom pro informaci o ID a dalsim tokenu nactenem po volani funkce
    int param_cnt = 0;
    P("--fcall");
    // fprintf(stderr, "--s: %s =\n", ta.attr.val);
    t_Node *temp = tableSearchItem(&table, ta.attr);
    if (temp == NULL){
        temp = tableInsertToken(&table, ta);
        temp->data->was_called = 1;
    }
    switch (tb.type){
        case T_LEFT_PAR:
            param1(&param_cnt);
            break;
        case T_ID:
        case T_DOUBLE:
        case T_INT:
        case T_STRING:
            // uz je prvni parametr funkce, ukladat nekam !!!
            param2(tb, &param_cnt);
            break;
        case T_EOL:
            break;
        default:
            PRINT_SYNTAX_ERROR("Function parameter");
    }
    printf("params: %d, expected: %d\n", param_cnt, temp->data->params_cnt);
    if (temp->data->defined == 1 && temp->data->params_cnt != param_cnt){
        fprintf(stderr, "PARAM_ERROR: %s called with %d params instead of %d on line: %d\n",
                stringGet(temp->data->name), param_cnt, temp->data->params_cnt, sc_line_cnt);
        exit(ERROR_SEM_PARAM);
    }
    //////////////////////
    //JESTE NEBYLA DEFINOVANA, ale byla zavolana
    //GENERACE FCALL INSTRUKCI
    //fprintf(stderr, "%s called with: %d params\n", stringGet(node->data->name) ,node->data->params_cnt);
}
void param1(int *param_cnt){
    P("--param1");
    int error = 0;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if (token.type == T_RIGHT_PAR){
        return;
    }else{
        //term se postara aby se program ukoncil s chybou
        term(token);
        if(param_cnt != NULL) (*param_cnt)++;
		if (pa_funcLocalTable != NULL && node != NULL)
		{
            P("__def param11");
			printToken(token, 0);
			if (arrParamAdd(node->data, token.attr) == MEMORY_ERROR) exit(ERROR_INTERNAL);

			t_Node *temp = tableInsertToken(node->data->local_symTable, token);
			tableChangeItemByNode(temp, 1, 0, 0, 0);
		}else{
            P("__def param11");

        }
        //###########################################
        param11(param_cnt);
    }
}
void param11(int *param_cnt){
    P("--param11");
    int error = 0;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if (token.type == T_COMMA){
        token = getNextToken(&error);
        CHECK_ERROR(error);
        term(token);
        //dalsi mozny parametr funkce, check v tabulce symbolu
        if(param_cnt != NULL) (*param_cnt)++;
		if (pa_funcLocalTable != NULL && node != NULL)
		{
            P("__def param11");
			printToken(token, 0);
			if (arrParamAdd(node->data, token.attr) == MEMORY_ERROR) exit(ERROR_INTERNAL);

			t_Node *temp = tableInsertToken(node->data->local_symTable, token);
			tableChangeItemByNode(temp, 1, 0, 1, 0);
            addInst(PI_DEFPARAM, (void*) temp, NULL, NULL , 0);
		//###########################################
        }else{
            P("__fcall param11");

        }
        param11(param_cnt);
    }else if (token.type == T_RIGHT_PAR){
        P("--token v param11 right par");
        token = getNextToken(&error);
        CHECK_ERROR(error);
        if (token.type == T_EOL){
            returnToken(token);

            return;
        }else{
            PRINT_SYNTAX_ERROR("EOL after PARAM");
        }
    }else{
        PRINT_SYNTAX_ERROR("Parameter");
    }
}

void param2(t_Token token, int *param_cnt){
    P("--param2");
    // int error = 0;
    // t_Token token = getNextToken(&error);
    // CHECK_ERROR(error);
    if (token.type == T_EOL){
        return;
    }else{
        term(token);
        if(pa_funcLocalTable == NULL){
            P("__fcall param2");
        }
        if(param_cnt != NULL) (*param_cnt)++;
        param22(param_cnt);
    }
}
void param22(int *param_cnt){
    P("--param22");
    int error = 0;
    t_Token token = getNextToken(&error);
    printToken(token,error);
    CHECK_ERROR(error);
    if (token.type == T_COMMA){
        token = getNextToken(&error);
        CHECK_ERROR(error);
        term(token);
        if(pa_funcLocalTable == NULL){
            P("__fcall param22");
        }
        if(param_cnt != NULL) (*param_cnt)++;
        param22(param_cnt);
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
            PRINT_SYNTAX_ERROR("TERM");
    }
}
void sec1(){
    P("--sec1");
    if (pa_funcLocalTable == NULL){ P("--global scope"); }else{P("--local scope")};
    int error = 0;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    while(token.type == T_EOL){
        token = getNextToken(&error);
        CHECK_ERROR(error);
        P("--token sec1");
        //printToken(token, error);
    }
    if (token.type == T_EOF){
        PRINT_SYNTAX_ERROR("END");
    }else if (token.type == T_DEF){
        PRINT_SYNTAX_ERROR("DEF not");
    }else if (token.type == T_END){
        token = getNextToken(&error);
        CHECK_ERROR(error);
        if (token.type == T_EOL){
            return;
        }else{
            PRINT_SYNTAX_ERROR("EOL after end");
        }
    }else if (token.type != T_EOL){
        code(token);
        sec1();
        P("--vratil sem se z sec1");
        return;
    }

}
void sec2(){
    P("--sec2");
    int error = 0;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    while(token.type == T_EOL){
        token = getNextToken(&error);
        CHECK_ERROR(error);
    }
    if(token.type == T_ELSE){
        P("--ifelse");
        t_Token token = getNextToken(&error);
        CHECK_ERROR(error);
        if (token.type == T_EOL){
            return;
        }else{
            PRINT_SYNTAX_ERROR("EOL after ELSE");
        }
    }else if (token.type == T_DEF){
        PRINT_SYNTAX_ERROR("DEF not");
    }else if (token.type != T_EOF && token.type != T_END){
        P("--dostal jsem se sem");
        code(token);
        sec2();
    }else{
        PRINT_SYNTAX_ERROR("ELSE");
    }
}
void code(t_Token token){
    P("--code");
    int ret_type = 0;
    int error = 0;
    switch (token.type){
        case T_IF:
            /* IF */
            P("--IF");
            token = getNextToken(&error);
            CHECK_ERROR(error);
            token = exprParse(token, token, pa_funcLocalTable, 0, &ret_type); //TODO
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
            token = exprParse(token, token, pa_funcLocalTable, 0, &ret_type);
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
        case T_NIL:
            returnToken(exprParse(token, token, pa_funcLocalTable, 0, &ret_type));
            break;
        case T_ID:
            {
            t_Token tb = getNextToken(&error);
            CHECK_ERROR(error);
            switch (tb.type){
                case T_ASSIGNMENT:
                    {
                    /* ID = */
                    assign(token, tb);
                    break;
                    }
                /* ID -> volani funkce */
                case T_ID:
                case T_LEFT_PAR:
                case T_DOUBLE:
                case T_INT:
                case T_STRING:
                case T_EOL:
                    // volani funkce
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
                case T_NIL:
                    /* expr */
                    returnToken(exprParse(token, tb, pa_funcLocalTable, 1, &ret_type));
                    break;
                default:
                    PRINT_SYNTAX_ERROR("Function call, assignment or expression");
            }
            break;
        }

        case T_EOL: P("--EOL") break;
        default: PRINT_SYNTAX_ERROR("SYMBOL not"); break;


    }
}

void program(){
    P("--program");
    int error = 0;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
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
		        t_Node *find = tableSearchItem(&table, token.attr);
                //jedna se o redefinici funkce
                if (find != NULL && find->data->defined == 1){
                    fprintf(stderr, "ERROR_SEMANTIC: Function redef: %s at line: %d\n", stringGet(&token.attr), sc_line_cnt);
                    exit(ERROR_SEMANTIC);
                }else{
    				node = tableInsertToken(&table, token);
    				if (node != NULL){
    					tableChangeItemByNode(node, 0, 0, 1, 1);
                        fprintf(stderr, "--fce: %s, %d\n", node->data->name->val, node->data->is_var);
                        pa_funcLocalTable = node->data->local_symTable;
    				}else{
    					exit(ERROR_INTERNAL); //todo
                    }
                }
				//#################################################
                addInst(PI_BEGINFUNC, (void*)node, NULL, NULL, 0);
                token = getNextToken(&error);
                CHECK_ERROR(error);
                if (token.type == T_LEFT_PAR){
                    param1(NULL);
                    token = getNextToken(&error);
                    CHECK_ERROR(error);
                    if (token.type != T_EOL){
                        PRINT_SYNTAX_ERROR("EOL");
                    }

                    sec1();
                    addInst(PI_ENDFUNC, NULL, NULL, NULL, 0);
                    pa_funcLocalTable = NULL;
                    program();
                }else{
                    PRINT_SYNTAX_ERROR("(");
                }
            }else{
                PRINT_SYNTAX_ERROR("ID");
            }
            break;
        default:
            code(token); //TODO check error
            program(); //TODO check error
            break;


    }
}
int main(){


    // ungetc('\n', stdin);
    // scannerInit();
    // //inicializace globalni tabulky symbolu
    // //program();
    // t_Token token;
    // do{
    //     int error;
    //     token = getPrintNextToken(&error);
    // }while(token.type != T_EOF);
    // scannerClean();
    //
    // ungetc('\n', stdin);



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
