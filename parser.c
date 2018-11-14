#include "scanner.h"
#include "ret_vals.h"

#define OK 0
#define ERROR 1
int error;

int term(t_Token token){
    printf("-term\n");
    if (token.type == ID || token.type == DOUBLE ||
        token.type == INT || token.type == STRING){
        return OK;
    }
    return ERROR;
}
int param11(){
    printf("-param11\n");
    t_Token token = getNextToken(&error);
    //printToken(token, error);
    if (token.type == RIGHT_PAR){
        //vrat dalsi, mozna eol
        token = getNextToken(&error);
        return token.type;
    }else if (token.type == COMMA){
        token = getNextToken(&error);
        if(term(token) == OK){
            printf("call: param11\n");
            param11();
        }else{
            printf("error wrong params line: %d\n", sc_line_cnt);
        }
    }
}
int param1(){
    printf("-param1\n");
    t_Token token = getNextToken(&error);
    //printToken(token, error);
    if (token.type == RIGHT_PAR){
        token = getNextToken(&error);
        //printToken(token, error);
        return token.type;
    }else if(term(token) == OK){
        if (param11() == T_EOL){
            return T_EOL;
        }else{
            printf("error eol expected line: %d\n", sc_line_cnt);
        }
    }else{
        printf("error right par or term expcted line: %d\n", sc_line_cnt);
    }
}

void fun_decl(){
    printf("-fun_decl\n");
    t_Token token = getNextToken(&error);
    //printToken(token, error);
    if (token.type == ID){
        token = getNextToken(&error);
        //printToken(token, error);
        if (token.type == LEFT_PAR){
            if (param1() == T_EOL){
                printf("-function decl correct line: %d\n", sc_line_cnt);
                return;
            }else{
                printf("error func decl line: %d\n", sc_line_cnt);
                return;
            }
        }else{
            printf("error lpar expected line: %d\n", sc_line_cnt);
            return;
        }
    }else{
        printf("error id expected line: %d\n", sc_line_cnt);
        return;
    }
}
void code(){
    //printf("CODE");
}
void program(t_Token token){
    switch (token.type){
        case T_EOF:
            return;
        case DEF:
            fun_decl();
            break;
        default:
            code();
            break;
    }
}

int main(){
    t_Token token;
    //pridani znaku noveho radku, na zacatek souboru, kvuli blokovym komentarum
    scannerInit();

    do {
        token = getNextToken(&error);
        program(token);
        //printToken(token, error);
    } while (token.type != T_EOF);


    scannerClean();
    return 0;
}
