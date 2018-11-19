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

void assign(){
    P("--assign");
    t_Token ta, tb;
    int error;
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
                exprParse(&ta, &tb);
                returnToken(ta);
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
    switch (tb.type){
        case T_LEFT_PAR:
            param1();
            break;
        case T_ID:
        case T_DOUBLE:
        case T_INT:
        case T_STRING:
            // uz je prvni parametr funkce, ukladat nekam !!!
            param2();
            break;
        case T_EOL:
            return;
        default:
            PRINT_SYNTAX_ERROR("Function parameter");
    }

}
void param1(){
    P("--param1");
    int error;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if (token.type == T_RIGHT_PAR){
        return;
    }else{
        //term se postara aby se program ukoncil s chybou
        term(token);
        param11();
    }
}
void param11(){
    P("--param11");
    int error;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if (token.type == T_COMMA){
        token = getNextToken(&error);
        CHECK_ERROR(error);
        term(token);
        //dalsi mozny parametr funkce, check v tabulce symbolu
        param11();
    }else if (token.type == T_RIGHT_PAR){
        return;
    }else{
        PRINT_SYNTAX_ERROR("Parameter");
    }
}

void param2(){
    P("--param2");
    int error;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if (token.type == T_EOL){
        return;
    }else{
        term(token);
        param22();
    }
}
void param22(){
    P("--param22");
    int error;
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
    int error;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if (token.type != T_END){
        code(token);
        sec1();
        // token = getPrintNextToken(&error);
        // CHECK_ERROR(error);
    }
    else if (token.type == T_END){
        P("--if else end");
        return;
    }else{
        PRINT_SYNTAX_ERROR("END");
    }

}
void sec2(){
    P("--sec2");
    int error;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    if (token.type != T_ELSE){
        code(token);
        sec2();
        // token = getNextToken(&error);
        // CHECK_ERROR(error);
    }
    else if (token.type == T_ELSE){
        P("--if else");
        token = getNextToken(&error);
        CHECK_ERROR(error);
        if (token.type == T_EOL){
            return;
        }else{
            PRINT_SYNTAX_ERROR("EOL");
        }
    }else{
        PRINT_SYNTAX_ERROR("ELSE");
    }
}
int isNextEol(){
    int error;
    t_Token token = getNextToken(&error);
    CHECK_ERROR(error);
    return (token.type == T_EOL) ? 1 : 0;
}
void code(t_Token token){
    P("--code");
    int error;
    switch (token.type){
        case T_IF:
            /* IF */
            P("--IF");
            token = getNextToken(&error);
            CHECK_ERROR(error);
            exprParse(&token, NULL);
            if (token.type == T_THEN){
                token = getNextToken(&error);
                CHECK_ERROR(error);
                if (token.type == T_EOL){
                    sec2();
                    sec1();
                }else{
                    PRINT_SYNTAX_ERROR("EOL");
                }
            }else{
                PRINT_SYNTAX_ERROR("THEN");
            }
            break;
        case T_WHILE:
            P("--WHILE");
            token = getNextToken(&error);
            CHECK_ERROR(error);
            exprParse(&token, NULL);
            if (token.type == T_DO){
                token = getNextToken(&error);
                CHECK_ERROR(error);
                if (token.type == T_EOL){
                    sec1();
                }else{
                    PRINT_SYNTAX_ERROR("EOL");
                }
            }else{
                PRINT_SYNTAX_ERROR("DO");
            }
            break;
        case T_LEFT_PAR:
        case T_INT:
        case T_DOUBLE:
        case T_STRING:
            exprParse(&token, NULL);
            returnToken(token);
            break;
        case T_ID:
            /* TODO dalsi prace s tabulkou symbolu*/
            {
            t_Token tb = getNextToken(&error);
            CHECK_ERROR(error);
            switch (tb.type){
                case T_ASSIGNMENT:
                /* ID = */
                assign();
                    break;
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
                    /* expr */
                    exprParse(&token, &tb);
                    returnToken(token);
                    break;
                default:
                    PRINT_SYNTAX_ERROR("Function call, assignment or expression");
            }
            break;
        }
    }
}

void program(){
    P("--program");
    int error;
    t_Token token = getNextToken(&error);
    //printToken(token, 0);
    CHECK_ERROR(error);
    //TODO????
    switch (token.type){
        case T_EOF:
            return;
        case T_DEF:
            P("--def");
            /* Definice funkce */
            token = getNextToken(&error);
            CHECK_ERROR(error);
            if (token.type == T_ID){
                //TODO pridat do tabulky symbolu zaznam o definici funkce
                token = getNextToken(&error);
                CHECK_ERROR(error);
                if (token.type == T_LEFT_PAR){
                    param1();
                    token = getNextToken(&error);
                    CHECK_ERROR(error);
                    if (token.type == T_EOL){
                        sec1(); //TODO check error
                        program(); //TODO check error
                    }else{
                        PRINT_SYNTAX_ERROR("EOL");
                    }
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
    scannerInit();
    //inicializace globalni tabulky symbolu
    program();
    scannerClean();
    return SUCCESS;
}
