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

string auxBuffer;

void scannerInit(){
    if (stringInit(&auxBuffer) == STR_ERROR){
        HANDLE_STR_ERROR;
    }
}
void scannerClean(){
    stringFree(&auxBuffer);
}

t_Token getNextToken(){
    t_Token token;  //token, ktery bude vracen

    string str;     //buffer pro identifikatory a kw
    stringInit(&str);
    stringPrint(&str);

    int state = 0;

    int cmnt_begin = 0;
    int cmnt_end = 0;
    while(42){
        //printf("DEBUG: State: %d\n", state);
        char symbol = getchar();
        switch (state) {
            case 0: //vychozi stav
                //printf("DEBUG: begin | znak: %c\n", symbol);
                if (isspace(symbol)){
                    state = 0;
                }else if (symbol == '#'){ //
                    state = 1;
                }else if (symbol == '='){
                    state = 2;
                }else if (symbol == '_' || islower(symbol)){
                    stringAddChar(&str, symbol);
                    state = 5;
                }
                break;
            case 1: //radkovy komentar
                //printf("DEBUG: komentar | znak: %c\n", symbol);
                if (symbol == '\n'){
                    state = 0;
                }else{
                    state = 1;
                }
                break;
            case 2: //'='
                //printf("DEBUG: '='| znak: %c\n", symbol);
                if (symbol == 'b'){ //moznost viceradkoveho komentare
                    stringAddChar(&str, symbol);
                    cmnt_begin = 1;
                    state = 5;
                }else if (symbol == '='){
                    token.type = EQ_REL;
                    token.attr = stringGet(&str);
                    state = 0;
                    return token;
                }else{
                    ungetc(symbol, stdin);
                    token.type = ASSIGNMENT;
                    token.attr = "";
                    state = 0;
                    return token;
                }
                break;//printf("DEBUG: '='| znak: %c\n", symbol);
            case 3: //=begin TODO
                if (symbol == '='){
                    stringClear(&str);
                    state = 4;
                }else{
                    state = 3;
                }
                break;

            case 4: //=
            //printf("DEBUG: Koment: Uz mozna koncim! %s\n", stringGet(&str));
                stringAddChar(&str, symbol);
                if (stringGetLength(&str) < 3){
                    state = 4;
                }else{
                    if (stringCompareConst(&str, "end") == 0){
                        state = 0;
                    }else{
                        state = 3;
                    }
                    stringClear(&str);
                }
                break;
            //TODO END

            case 5: //identifikator / klicove slovo
                //printf("DEBUG: 'ID/KW'| znak: %c | buffer: %s\n ", symbol, stringGet(&str));
                if (isalnum(symbol) || symbol == '_'){
                    stringAddChar(&str, symbol);
                    stringPrint(&str);
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
                    state = 6;
                }else{
                    ungetc(symbol, stdin);
                    token.type = ID;
                    token.attr = stringGet(&str);
                    return token;
                }
                break;
            case 6: //indetifikator + ? nebo ! (mozny konec identifikatoru)
                //printf("DEBUG: mozny konec id\n");

                break;
        }
    }

    stringFree(&str);
}
