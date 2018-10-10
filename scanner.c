/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Implementace funkci pro string
*/

#include "scanner.h"
#include "string.h"
#include "ret_vals.h"

//Globalni promenne pro "zbyly token"
string restBuffer;
int restType;
int restTokenAvailable = 0;

void scannerInit(){
    if (stringInit(&restBuffer) == STR_ERROR){
        HANDLE_STR_ERROR;
    }
}
void scannerClean(){
    stringFree(&restBuffer);
}

t_Token getRestToken(){
    t_Token tmp;
    tmp.type = restType; tmp.attr = stringGet(&restBuffer);
    return tmp;
}

t_Token getNextToken(){
    t_Token token;  //token, ktery bude vracen

    string str;     //buffer pro identifikatory a kw
    stringInit(&str);
    if (restTokenAvailable){
        return getRestToken();
    }

    int state = 0;

    while(42){
        //printf("DEBUG: State: %d\n", state);
        char symbol = getchar();
        switch (state) {
            case S_START: //vychozi stav
                //printf("DEBUG: begin | znak: %c\n", symbol);
                if (isspace(symbol)){
                    state = S_START;
                }else if (symbol == '#'){ //
                    state = S_LINE_COMMENT;
                }else if (symbol == '='){
                    state = S_EQUALS;
                }else if (symbol == '_' || islower(symbol)){
                    stringAddChar(&str, symbol);
                    state = S_ID_KW;
                }else if (symbol == '<'){
                    state = S_LESS;
                }else if (symbol == '>'){
                    state = S_MORE;
                }else if (symbol == '!'){
                    state = S_NOT_EQ;
                }else if (symbol == '*'){
                    token.type = MUL; token.attr = "";
                    return token;
                }else if (symbol == '/'){
                    token.type = DIV; token.attr = "";
                    return token;
                }else if (symbol == '+'){
                    token.type = PLUS; token.attr = "";
                    return token;
                }else if (symbol == '-'){
                    token.type = MINUS; token.attr = "";
                    return token;
                }else if (symbol == '('){
                    token.type = LEFT_PAR; token.attr = "";
                    return token;
                }else if (symbol == ')'){
                    token.type = RIGHT_PAR; token.attr = "";
                    return token;
                }else if (symbol == '"'){
                    state = S_STRING;
                }
                break;
            case S_LINE_COMMENT: //radkovy komentar
                //printf("DEBUG: komentar | znak: %c\n", symbol);
                if (symbol == '\n'){
                    state = S_START;
                }else{
                    state = S_LINE_COMMENT;
                }
                break;
            case S_EQUALS: //'='
                //printf("DEBUG: '='| znak: %c\n", symbol);
                if (symbol == 'b'){ //moznost viceradkoveho komentare
                    stringAddChar(&str, symbol);
                    state = S_BC_BEGIN;
                }else if (symbol == '='){
                    token.type = EQ_REL; token.attr = stringGet(&str);
                    state = S_START;
                    return token;
                }else{
                    ungetc(symbol, stdin);
                    token.type = ASSIGNMENT; token.attr = "";
                    state = S_START;
                    return token;
                }
                break;
            case S_BC_BEGIN: //pokud nactes b tak zkus, zda neni zacatek blokoveho komentare

                break;

            case S_BLOCK_COMMENT: //zustan dokud nenarazis na end

                break;

            case S_ID_KW: //identifikator / klicove slovo
                //printf("DEBUG: 'ID/KW'| znak: %c | buffer: %s\n ", symbol, stringGet(&str));
                if (isalnum(symbol) || symbol == '_'){
                    stringAddChar(&str, symbol);
                    stringPrint(&str);
                    state = S_ID_KW;
                    if (stringCompareConst(&str, "def") == 0){ token.type = KW; token.attr = "def"; return token;}
                    else if (stringCompareConst(&str, "do") == 0){ token.type = KW; token.attr = "do"; return token;}
                    else if (stringCompareConst(&str, "else") == 0){ token.type = KW; token.attr = "else"; return token;}
                    else if (stringCompareConst(&str, "end") == 0){ token.type = KW; token.attr = "end"; return token;}
                    else if (stringCompareConst(&str, "if") == 0){ token.type = KW; token.attr = "if"; return token;}
                    else if (stringCompareConst(&str, "not") == 0){ token.type = KW; token.attr = "not"; return token;}
                    else if (stringCompareConst(&str, "nil") == 0){ token.type = KW; token.attr = "nil"; return token;}
                    else if (stringCompareConst(&str, "then") == 0){ token.type = KW; token.attr = "then"; return token;}
                    else if (stringCompareConst(&str, "while") == 0){ token.type = KW; token.attr = "while"; return token;}
                }else if (symbol == '?' || symbol == '!'){
                    stringAddChar(&str, symbol);
                    token.type = ID; token.attr = stringGet(&str);
                    return token;
                }else{
                    ungetc(symbol, stdin);
                    token.type = ID; token.attr = stringGet(&str);
                    return token;
                }
                break;

            case S_LESS:
                token.attr = "";
                if (symbol == '='){
                    token.type = LESS_EQ;
                    return token;
                }else{
                    token.type = LESS;
                    ungetc(symbol, stdin);
                    return token;
                }
                break;

            case S_MORE:
                token.attr = "";
                if (symbol == '='){
                    token.type = MORE_EQ;
                    return token;
                }else{
                    token.type = MORE;
                    ungetc(symbol, stdin);
                    return token;
                }
                break;

            case S_NOT_EQ:
                if (symbol == '='){
                    token.type = NOT_EQ;
                    token.attr = "";
                    return token;
                }else{
                    fprintf(stderr, "LEX_ERR\n");
                    exit(LEX_ERR);
                }
                break;
        }
    }

    stringFree(&str);
}
