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

t_Token getNextToken(){
    t_Token token;  //token, ktery bude vracen

    string str;     //buffer pro identifikatory a kw
    stringInit(&str);

    int state = 0;

    while(42){
        //printf("DEBUG: State: %d\n", state);
        char symbol = getc(stdin);
        switch (state) {
            case S_START: //vychozi stav, pokud nacte lexem s delkou jedna, vrati ho
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
                }else if (isdigit(symbol)){
                    stringAddChar(&str, symbol);
                    state = S_DIGIT;
                }else if (symbol == '<'){
                    state = S_LESS;
                }else if (symbol == '>'){
                    state = S_MORE;
                }else if (symbol == '!'){
                    state = S_NOT_EQ;
                }else if (symbol == '"'){
                    state = S_STRING;
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
                }else if (symbol == ','){
                    token.type = COMMA, token.attr = "";
                }
                break;
            case S_LINE_COMMENT: // #radkovy komentar
                //printf("DEBUG: komentar | znak: %c\n", symbol);
                if (symbol == '\n'){
                    state = S_START;
                }else{
                    state = S_LINE_COMMENT;
                }
                break;
            case S_EQUALS: // =, =b, ==
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
                if (isalnum(symbol) || symbol == '_'){
                    stringAddChar(&str, symbol);
                    stringPrint(&str);
                    state = S_ID_KW;
                    if (stringCompareConst(&str, "def") == 0){ token.type = DEF; return token;}
                    else if (stringCompareConst(&str, "do") == 0){ token.type = DO; return token;}
                    else if (stringCompareConst(&str, "else") == 0){ token.type = ELSE; return token;}
                    else if (stringCompareConst(&str, "end") == 0){ token.type = END; return token;}
                    else if (stringCompareConst(&str, "if") == 0){ token.type = IF; return token;}
                    else if (stringCompareConst(&str, "not") == 0){ token.type = NOT; return token;}
                    else if (stringCompareConst(&str, "nil") == 0){ token.type = NIL; return token;}
                    else if (stringCompareConst(&str, "then") == 0){ token.type = THEN; return token;}
                    else if (stringCompareConst(&str, "while") == 0){ token.type = WHILE; return token;}
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

            case S_LESS:    // <, <=
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

            case S_MORE:    // >, >=
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

            case S_NOT_EQ: //!=
                if (symbol == '='){
                    token.type = NOT_EQ;
                    token.attr = "";
                    return token;
                }else{
                    fprintf(stderr, "LEX_ERR\n");
                    exit(LEX_ERROR);
                }
                break;

            case S_STRING:
                if (symbol == '"'){
                    token.type = STR; token.attr = stringGet(&str);
                    return token;
                }else if (symbol > 31){ //znaky vetsi nez 31
                    state = S_STRING;
                }else if (symbol == '\\'){
                    //specialni znak
                    printf("Specialek\n");
                }
                break;

            case S_DIGIT:
                break;
        }
    }

    stringFree(&str);
}
