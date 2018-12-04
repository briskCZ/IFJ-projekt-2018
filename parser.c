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


#define PRINT_SYNTAX_ERROR(symbol) {fprintf(stderr, "ERROR_SYNTAX: %s expected on line %d\n", symbol, sc_line_cnt);\
                                   cleanAll(); exit(ERROR_SYNTAX);}
#define SAVE_ERROR(error){if (ERROR == 0) ERROR = error;}

int isGlobal(){
    return (pa_funcLocalTable == NULL) ? 1 : 0;
}
t_symTable* getScopeTable(){
    return (isGlobal()) ? &table : pa_funcLocalTable;
}

void isQmExc(t_Token token){
    //kontrola zda neni na posledni pozici ? nebo !
    string *name = &token.attr;
    if (name->val[name->length - 1] == '?' || name->val[name->length - 1] == '!'){
        cleanAll();
        fprintf(stderr, "ERROR_LEX: Cannot assign variable to id with ? or ! on line: %d\n", sc_line_cnt);
        exit(ERROR_LEX);
    }
}
/* Muzu do leve strany priradit? */
t_Node* isAssignable(t_Token left, int type, int *isNew, t_symTable *scope){
    t_Node* tmp = tableSearchItem(&table, left.attr);
    if (tmp != NULL && tmp->data->defined){
        *isNew = 0;
        if (tmp->data->is_var == 0){
            fprintf(stderr, "ERROR_SEMANTIC: Cannot assign to function %s on line: %d\n", left.attr.val, sc_line_cnt);
            cleanAll();
            exit(ERROR_SEMANTIC);
        }
    }else{
        tmp = tableSearchItem(scope, left.attr);
        if (tmp == NULL){
            tmp = tableInsertToken(scope, left);
            if (tmp->data->data_type != T_PARAM){
                tableChangeItemByNode(tmp, 1, -1, 1, isGlobal());
            }
            *isNew = 1;
        }else{
            *isNew = 0;
        }
    }
    return tmp;
}
/* instrukce pro generator, podle toho zda je leva strana nova a jedna se o volani funkce*/
void f_callAssIns(int isNew, t_Node *left, t_Node *right){
    if (isNew){
        if (pa_while || pa_if){
            addInst(PI_WHILE_DECL_FUNC, (void*)left, (void*)right, NULL, 1);
            addInst(PI_ASS_FUNC, (void*)left, (void*)right, NULL, 0);
        }else{
            addInst(PI_ASS_DECL_FUNC, (void*)left, (void*)right, NULL, 0);
        }
    }else{
        addInst(PI_ASS_FUNC, (void*)left, (void*)right, NULL, 0);
    }
}

/* instrukce pro generator, podle toho zda je leva strana nova pri prirazeni*/
void assIns(int isNew, t_Node *left){
    if(isNew){
        if (pa_while || pa_if){
            addInst(PI_WHILE_DECL, (void*)left, NULL, NULL, 1);
            addInst(PI_ASS, (void*)left, NULL, NULL, 0);
        }else{
            addInst(PI_ASS_DECL, (void*)left, NULL, NULL, 0);
        }
    }else{
        addInst(PI_ASS, (void*)left, NULL, NULL, 0);
    }
}

void assign(t_Token left){
    isQmExc(left);
    P(">>assign");
    int isNew = 0, ret_type = 0;
    t_Token ta, tb;
    ta = tarrGetNextToken(&token_array);
    if (ta.type == T_ID){
        tb = tarrGetNextToken(&token_array);
        switch (tb.type){
            /* ID = ID EOL(bud prirazeni promenne do id, nebo volani funkce)*/
            case T_EOL:{
                P("--asign jedne promenne");
                if (!isGlobal()){
                    P(">a>local_scope");
                    t_Node *leftVar = isAssignable(left, ta.type, &isNew, pa_funcLocalTable);
                    t_Node *rightVar = tableSearchItem(&table, ta.attr);
                    if (rightVar != NULL && rightVar->data->is_var == 0){
                        //byla nalezena a je funkce
                        P(">a>local_scope>func_call");
                        node = rightVar;
                        f_call(ta, tb);
                        f_callAssIns(isNew, leftVar, rightVar);
                        tableChangeItemByNode(leftVar, 1, T_PARAM, 1, 0);
                        return;
                    }else{
                    /* polozka nebyla nalezena v globalni tabulce - neni funkce*/
                        rightVar = tableSearchItem(pa_funcLocalTable, ta.attr);
                        if (rightVar != NULL){
                            /* promenan je parametr a nebo lokalni */
                            P(">a>local_scope>param|locvar");
                            exprParse(ta, tb, pa_funcLocalTable, 1, &ret_type);
                            tableChangeItemByNode(leftVar, 1, ret_type, 1, 0);
                            assIns(isNew, leftVar);
                            return;
                        }else if (stringCompare(&left.attr, &ta.attr) == 0){
                            /* osetreni moznosti a=a*/
                            P(">a>local_scope>a=a");
                            ta.type = T_NIL;
                            exprParse(ta, tb, pa_funcLocalTable, 1, &ret_type);
                            tableChangeItemByNode(leftVar, 1, ret_type, 1, 0);
                            assIns(isNew, leftVar);
                            return;
                        }else{
                            fprintf(stderr, "ERROR_SEMANTIC: Variable: %s not defined on line: %d\n",  ta.attr.val, sc_line_cnt);
                            cleanAll();
                            exit(ERROR_SEMANTIC);
                        }
                    }
                }else{
                    P(">a>global_scope");
                    t_Node *rightVar = tableSearchItem(&table, ta.attr);
                    if (rightVar != NULL){
                        t_Node *leftVar = isAssignable(left, ta.type, &isNew, &table);
                        if (rightVar->data->is_var){
                            /* promenna */
                            P(">a>global_scope>a=var");
                            exprParse(ta, tb, NULL, 1, &ret_type);
                            tableChangeItemByNode(leftVar, 1, ret_type, 1, 1);
                            assIns(isNew, leftVar);
                            return;
                        }else if (rightVar->data->defined){
                            /* funkce */
                            P(">a>global_scope>a=fcall");
                            f_call(ta, tb);
                            f_callAssIns(isNew, leftVar, rightVar);
                            tableChangeItemByNode(leftVar, 1, T_PARAM, 1, 0);
                            return;
                        }else{
                            fprintf(stderr, "ERROR_SEMANTIC: Variable %s not defined on line: %d\n", rightVar->data->name->val, sc_line_cnt);
                            cleanAll();
                            exit(ERROR_SEMANTIC);
                        }
                    }else{
                        /* a = a */
                        P(">a>global_scope>a=a")
                        if (stringCompare(&left.attr, &ta.attr) == 0){
                            ta.type = T_NIL;
                            //promenna je urcite nova, nebyla v tabulce symbolu
                            t_Node *leftVar = isAssignable(left, ta.type, &isNew, &table);
                            exprParse(ta, tb, NULL, 1, &ret_type);
                            tableChangeItemByNode(leftVar, 1, ret_type, 1, 1);
                            assIns(isNew, leftVar);
                            return;
                        }else{
                            fprintf(stderr, "ERROR_SEMANTIC: Variable not defined: %s on_line: %d\n", ta.attr.val, sc_line_cnt);
                            cleanAll();
                            exit(ERROR_SEMANTIC);
                        }
                    }
                }
            }break;
            /* ID = ID ID, ID = ID (, ID = ID "string" -> volani funkce*/
            case T_ID:
            case T_LEFT_PAR:
            case T_FLOAT:
            case T_INT:
            case T_NIL:
            case T_STRING:{
                t_Node *leftVar = isAssignable(left, ta.type, &isNew, getScopeTable());
                t_Node *rightVar = tableSearchItem(&table, ta.attr);
                if (rightVar == NULL){
                    fprintf(stderr, "ERROR_SEMANTIC: Function not defined: %s on line %d\n", ta.attr.val, sc_line_cnt);
                    cleanAll();
                    exit(ERROR_SEMANTIC);
                }else if(isGlobal() && rightVar->data->defined == 0){
                    fprintf(stderr, "ERROR_SEMANTIC: Function not defined above: %s on line %d\n", ta.attr.val, sc_line_cnt);
                    cleanAll();
                    exit(ERROR_SEMANTIC);
                }else if (rightVar->data->is_var){
                    fprintf(stderr, "ERROR_SEMANTIC: %s is var, not function, line %d\n", stringGet(rightVar->data->name), sc_line_cnt);
                    cleanAll();
                    exit(ERROR_SEMANTIC);
                }else{
                    P(">a>fcall s parametry");
                    node = rightVar;
                    f_call(ta, tb);
                    f_callAssIns(isNew, leftVar, rightVar);
                    tableChangeItemByNode(leftVar, 1, T_PARAM, 1, 0);
                    return;
                }
            }
            break;
            /* ID = ID + -> prirazeni vyrazu, ktery zacina promennou */
            case T_PLUS:
            case T_MINUS:
            case T_MUL:
            case T_DIV:
            case T_LESS:
            case T_LESS_EQ:
            case T_MORE:
            case T_MORE_EQ:
            case T_EQ_REL:
            case T_NOT_EQ:{
                P(">a>expr");
                t_symTable *tablet = getScopeTable();
                t_Node *leftVar = isAssignable(left, ta.type, &isNew, getScopeTable());
                t_Token t = exprParse(ta, tb, pa_funcLocalTable, 1, &ret_type);
                if (t.type != T_EOL) PRINT_SYNTAX_ERROR("EOL after expr assign");
                tableChangeItemByNode(leftVar, 1, ret_type, 1, isGlobal());
                assIns(isNew, leftVar);
                return;
            }break;
            default:
                PRINT_SYNTAX_ERROR("ID = ID (as fcall) or ID = ID (as expr)");
            break;
        }
    /* ID = (, ID = 5, ID = nil -> prirazeni vyrazu */
    }else if (ta.type == T_LEFT_PAR || ta.type == T_INT || ta.type == T_FLOAT ||
              ta.type == T_STRING || ta.type == T_NIL){
        P(">a>term");
        t_Node *leftVar = isAssignable(left, ta.type, &isNew, getScopeTable());
        t_Token t = exprParse(ta, ta, getScopeTable(), 0, &ret_type);
        if (t.type != T_EOL) PRINT_SYNTAX_ERROR("EOL after term assign");
        tableChangeItemByNode(leftVar, 1, ret_type, 1, isGlobal());
        assIns(isNew, leftVar);
    }else{
        PRINT_SYNTAX_ERROR("ID = FCALL or ID = EXPR");
    }
}

void f_call(t_Token ta, t_Token tb){
    //ta a tb jenom pro informaci o ID a dalsim tokenu nactenem po volani funkce
    int param_cnt = 0;
    t_Elem *prevActive = list->act;
    P("--fcall");
    /* pripadne vlozeni do tabulky symbolu */
    t_Node *temp = tableSearchItem(&table, ta.attr);
    if (temp == NULL){
        fprintf(stderr, "ERROR_SEMANTIC: Undefined function call %s on_line %d\n", ta.attr.val, sc_line_cnt);
        cleanAll();
        exit(ERROR_SEMANTIC);
    }else{
        if (isGlobal()){
            if (!temp->data->defined){
                fprintf(stderr, "ERROR_SEMANTIC: Undefined function call %s on_line %d\n", ta.attr.val, sc_line_cnt);
                cleanAll();
                exit(ERROR_SEMANTIC);
            }
        }
    }
    node = temp;
    /* pokud neni print generuj ramec, pzn. print se vola na kazdy parametr */
    if(stringCompareConst(&ta.attr, "print") != 0){
        addInst(PI_FCALL, (void*)temp, NULL, NULL, 0);
        setActive(list->last);
        addInst(INS_CREATEFRAME, (void*)temp, NULL, NULL, 1);
    }
    switch (tb.type){
        case T_LEFT_PAR:
            param1(&param_cnt);
            break;
        case T_ID:
        case T_FLOAT:
        case T_INT:
        case T_STRING:
        case T_NIL:
            // uz je prvni parametr funkce, ukladat nekam !!!
            param2(tb, &param_cnt);
            break;
        case T_EOL:
            break;
        default:
            PRINT_SYNTAX_ERROR("Function parameter");
    }
    /* TODO znova pokud je funkce volana poprve a jeste neni definovana */

    if(stringCompareConst(&ta.attr, "print") == 0) param_cnt = -1;
    if (temp->data->defined == 1 && temp->data->params_cnt != param_cnt){
        fprintf(stderr, "PARAM_ERROR: %s called with %d params instead of %d on line: %d\n",
                    stringGet(temp->data->name), param_cnt, temp->data->params_cnt, sc_line_cnt-1);
        cleanAll();
        exit(ERROR_SEM_PARAM);
    }
    setActive(prevActive);

}
void paramHandler(t_Token token, int param_cnt){
    if (node == NULL){
        fprintf(stderr, "Error: NODE is null...\n");
        exit(ERROR_INTERNAL);
    }
    if (node->data->defined == 1){
        //aktualne pouzivana tabulka symbolu
        t_symTable *scopeTable = getScopeTable();
        if (token.type == T_ID){
            t_Node *param = tableSearchItem(scopeTable, token.attr);
            /* zda je prametr definovany v localnim scope*/
            if (param != NULL && param->data->is_var){
                /* pokud je print, volam funkci pro kazdy parametr */
                if (stringCompareConst(node->data->name, "print") == 0){
                    addInst(PI_FCALL, (void*)node, NULL, NULL, 0);
                    setActive(list->last);
                    addInst(INS_CREATEFRAME, (void*)node, NULL, NULL, 1);
                    addInst(PI_FCALL_PARAMID, (void*)param, (void*)1, (void*)param->data->data_type, 1);
                }else{
                    addInst(PI_FCALL_PARAMID, (void*)param, (void*)param_cnt, (void*)param->data->data_type, 1);
                }
            }else{
                fprintf(stderr, "ERROR_SEMANTIC: Variable: %s not defined on line: %d\n", stringGet(&token.attr), sc_line_cnt);
                cleanAll();
                exit(ERROR_SEMANTIC);
            }
        }else{
            /* pokud je print, volam funkci pro kazdy parametr */
            if (stringCompareConst(node->data->name, "print") == 0){
                addInst(PI_FCALL, (void*)node, NULL, NULL, 0);
                setActive(list->last);
                addInst(INS_CREATEFRAME, (void*)node, NULL, NULL, 1);
                addInst(PI_FCALL_PARAMT, (void*)token.attr.val, (void*)1, (void*)token.type, 1);
            }else{
                addInst(PI_FCALL_PARAMT, (void*)token.attr.val, (void*)param_cnt, (void*)token.type, 1);
            }
        }
    }
}
/* pridani parametru do lokalni tabulky symbolu v definici*/
void paramDefHandler(t_Token token, int param_cnt){

    t_Node *temp = tableSearchItem(pa_funcLocalTable, token.attr);
    if (temp == NULL){
        temp = tableInsertToken(pa_funcLocalTable, token);
        addInst(PI_DEF_PARAM, (void*)temp, (void*)param_cnt, NULL , 0);
        tableChangeItemByNode(temp, 1, T_PARAM, 1, 0);
    }else{
        fprintf(stderr, "ERROR_SEMANTIC: Duplicate parameter name %s on line: %d\n", token.attr.val, sc_line_cnt);
        cleanAll();
        exit(ERROR_SEMANTIC);
    }
}
void param1(int *param_cnt){
    P("--param1");
    t_Token token = tarrGetNextToken(&token_array);
    if (token.type == T_RIGHT_PAR){
        token = tarrGetNextToken(&token_array);
    }else{
        //term se postara aby se program ukoncil s chybou
        if (node != NULL && !node->data->defined){
            if (token.type != T_ID)
                PRINT_SYNTAX_ERROR("ID");
        }else{
            term(token);
        }
        P("--node pred def v param1");
		if (pa_funcLocalTable != NULL && node != NULL && !node->data->defined)
		{
            P("__def param11");
            (*param_cnt)++;
            paramDefHandler(token, *param_cnt);

		}else{
            P("call param11");
            (*param_cnt)++;
            paramHandler(token, *param_cnt);

        }
        param11(param_cnt);
    }
}
void param11(int *param_cnt){
    P("--param11");
    t_Token token = tarrGetNextToken(&token_array);
    if (token.type == T_COMMA){
        token = tarrGetNextToken(&token_array);
        if (node != NULL && !node->data->defined){
            if (token.type != T_ID)
                PRINT_SYNTAX_ERROR("ID in param definition");
        }else{
            term(token);
        }
        //dalsi mozny parametr funkce, check v tabulce symbolu
        if (pa_funcLocalTable != NULL && node != NULL && !node->data->defined)
		{
            P("__def param11");
            (*param_cnt)++;
            paramDefHandler(token, *param_cnt);
        }else{
            P("__fcall param11");
            (*param_cnt)++;
            paramHandler(token, *param_cnt);

        }
        param11(param_cnt);
    }else if (token.type == T_RIGHT_PAR){
        P("--token v param11 right par");
        token = tarrGetNextToken(&token_array);
        if (token.type == T_EOL){
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
    // t_Token token = tarrGetNextToken(&token_array);
    if (token.type == T_EOL){
        return;
    }else{
        P("__fcall param2");
        term(token);
        //pokud je definovana , TODO pokud neni definovana ale muze byt
        (*param_cnt)++;
        paramHandler(token, *param_cnt);
        param22(param_cnt);
    }
}
void param22(int *param_cnt){
    P("--param22");
    t_Token token = tarrGetNextToken(&token_array);
    if (token.type == T_COMMA){
        token = tarrGetNextToken(&token_array);
        term(token);
        (*param_cnt)++;
        paramHandler(token, *param_cnt);
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
        case T_FLOAT:
        case T_STRING:
        case T_NIL:
            return;
        default:
            PRINT_SYNTAX_ERROR("TERM");
    }
}
void sec1(){
    P("--sec1");
    t_Token token = tarrGetNextToken(&token_array);
    while(token.type == T_EOL){
        token = tarrGetNextToken(&token_array);
    }
    if (token.type == T_EOF){
        PRINT_SYNTAX_ERROR("END");
    }else if (token.type == T_DEF){
        PRINT_SYNTAX_ERROR("DEF not");
    }else if (token.type == T_END){
        token = tarrGetNextToken(&token_array);
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
    t_Token token = tarrGetNextToken(&token_array);
    while(token.type == T_EOL){
        token = tarrGetNextToken(&token_array);
    }
    if(token.type == T_ELSE){
        P("--ifelse");
        t_Token token = tarrGetNextToken(&token_array);
        if (token.type == T_EOL){
            return;
        }else{
            PRINT_SYNTAX_ERROR("EOL after ELSE");
        }
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
    switch (token.type){
        case T_IF:
            /* IF */
            P("--IF");
            token = tarrGetNextToken(&token_array);
            token = exprParse(token, token, pa_funcLocalTable, 0, &ret_type); //TODO
            addInst(PI_IF_START, (void*)pa_if_count, NULL, NULL, 0);
            pa_if_count++;
            if (pa_while == 0 && pa_if == 0 ){
                setActive(list->last);
            }
            if (token.type == T_THEN){
                pa_if = 1;
                P("--then");
                token = tarrGetNextToken(&token_array);
                if (token.type == T_EOL){
                    sec2();
                    pa_if_count--;
                    addInst(PI_IF_ELSE, (void*)pa_if_count, NULL, NULL, 0);
                    pa_if_count++;
                    sec1();
                    pa_if = 0;
                    pa_if_count--;
                    addInst(PI_IF_END, (void*)pa_if_count, NULL, NULL, 0);
                }else{
                    PRINT_SYNTAX_ERROR("EOL after THEN");
                }
            }else{
                PRINT_SYNTAX_ERROR("THEN");
            }
            break;
            /* KONEC IF */
        case T_WHILE:
            P("--WHILE");
            /* WHILE */
            token = tarrGetNextToken(&token_array);
            if (pa_while_count == 0 && pa_if == 0){
                setActive(list->last);
            }
            addInst(PI_WHILE_START, (void*)pa_while_count, NULL, NULL, 0);
            pa_while_count++;
            token = exprParse(token, token, pa_funcLocalTable, 0, &ret_type);
            addInst(PI_WHILE_EX, NULL, NULL, NULL, 0);
            if (token.type == T_DO){
                pa_while = 1;
                token = tarrGetNextToken(&token_array);
                if (token.type == T_EOL){
                    sec1();
                    pa_while = 0;
                    pa_while_count--;
                    addInst(PI_WHILE_END, (void*)pa_while_count, NULL, NULL, 0);

                }else{
                    PRINT_SYNTAX_ERROR("EOL after DO");
                }
            }else{
                PRINT_SYNTAX_ERROR("DO");
            }
            break;
            /* KONEC WHILE */
        case T_LEFT_PAR:
        case T_INT:
        case T_FLOAT:
        case T_STRING:
        case T_NIL:
            P("--kratky random vyraz");
            exprParse(token, token, pa_funcLocalTable, 0, &ret_type);
            tarrReturnToken(&token_array);
            break;
        case T_ID:{
            t_Token tb = tarrGetNextToken(&token_array);
            switch (tb.type){
                case T_ASSIGNMENT:
                    {
                    /* ID = */
                    assign(token);
                    break;
                    }
                /* ID -> volani funkce */
                case T_ID:
                case T_LEFT_PAR:
                case T_FLOAT:
                case T_INT:
                case T_STRING:
                    // volani funkce
                    f_call(token, tb);
                    break;
                case T_EOL:{
                    /* volani funkce bez parametru jinak promenna */
                    t_Node *var = tableSearchItem(getScopeTable(), token.attr);
                    if (var != NULL && var->data->defined){
                        if (var->data->is_var){
                            tableChangeItemByNode(var, 1, token.type, -1, isGlobal());
                            exprParse(token, tb, pa_funcLocalTable, 1, &token.type);
                            tarrReturnToken(&token_array);
                        }else{
                            f_call(token, tb);
                        }
                    }else{
                        fprintf(stderr, "ERROR_SEMANTIC: Using not defined: %s on line: %d\n", token.attr.val, sc_line_cnt-1);
                        cleanAll();
                        exit(ERROR_SEMANTIC);
                    }
                    break;
                }
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
                    P("--random vyraz");
                    exprParse(token, tb, pa_funcLocalTable, 1, &ret_type);
                    tarrReturnToken(&token_array);
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
    t_Token token = tarrGetNextToken(&token_array);
    t_Node *funcNode = NULL;
    switch (token.type){
        case T_EOF:
            return;
        case T_DEF:
            /* Definice funkce */
            P("--def");
            token = tarrGetNextToken(&token_array);
            int def_params_cnt = 0;
            if (token.type == T_ID){
                //TODO pridat do tabulky symbolu zaznam o definici funkce
		        t_Node *find = tableSearchItem(&table, token.attr);
                //jedna se o redefinici funkce
                if (find != NULL && find->data->defined == 1){
                    fprintf(stderr, "ERROR_SEMANTIC: Function redef: %s at line: %d\n", stringGet(&token.attr), sc_line_cnt);
                    cleanAll();
                    exit(ERROR_SEMANTIC);
                }else{
    				node = tableInsertToken(&table, token);
                    funcNode = node;
    				if (node != NULL){
                        //fprintf(stderr, "--fce: %s, %d\n", node->data->name->val, node->data->is_var);
                        pa_funcLocalTable = node->data->local_symTable;
    				}else{
    					exit(ERROR_INTERNAL); //todo
                    }
                }
                addInst(PI_BEGINFUNC, (void*)node, NULL, NULL, 0);
                token = tarrGetNextToken(&token_array);
                if (token.type == T_LEFT_PAR){
                    //param11 pohlida eol za )
                    param1(&def_params_cnt);
                    tableChangeItemByNode(node, 0, 0, 1, 1);
                    sec1();
                    node = funcNode;
                    addInst(PI_ENDFUNC, (void*)node, NULL, NULL, 0);
                    pa_funcLocalTable = NULL;
                    program();
                }else{
                    PRINT_SYNTAX_ERROR("(");
                }
            }else{
                PRINT_SYNTAX_ERROR("ID");
            }
            /* KONEC DEFINICE FUNKCE */
            break;
        default:
            code(token); //TODO check error
            program(); //TODO check error
            break;


    }
}
/* Vlozeni vestavenych funkci do tabulky symbolu */
void addSingleBuiltin(char* name, int params_cnt){
    t_Node *n;
    t_Token toadd;
    toadd.type = T_ID;
    stringInit(&toadd.attr);
    stringInsert(&toadd.attr, name);
    n = tableInsertToken(&table, toadd);
    tableChangeItemByNode(n, 0, 0, 1, 1);
    addInst(PI_BUILTFUNC, (void*)n, NULL, NULL, 0);
    n->data->params_cnt = params_cnt;
    stringFree(&toadd.attr);
}
void addBuiltins(){
    addSingleBuiltin("inputs", 0);
    addSingleBuiltin("inputi", 0);
    addSingleBuiltin("inputf", 0);
    addSingleBuiltin("print", -1);
    addSingleBuiltin("length", 1);
    addSingleBuiltin("substr", 3);
    addSingleBuiltin("ord", 2);
    addSingleBuiltin("chr", 1);
}

void cleanAll(){
    tarrFree(&token_array);
    tableDestroy(&table);
    freeList();
    scannerClean();
}
int main(){
    pa_if_count = 0;
    pa_while_count = 0;
    scannerInit();
    table = tableInit();
    listInit();
    tarrInit(&token_array);
    tarrFill(&token_array);
    tarrGetFuncInfo(&token_array);
    addBuiltins();
    program();
    generate();
    cleanAll();
    return SUCCESS;
}
