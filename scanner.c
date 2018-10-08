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

t_Token getNextToken(){
    t_Token token;  //token, ktery bude vracen

    string str;     //buffer pro identifikatory a kw
    stringInit(&str);
    stringPrint(&str);

    int state = 0;

    int cmnt_begin = 0;
    int cmnt_end = 0;
    while(42){
        printf("DEBUG: State: %d\n", state);
        char symbol = getchar();
        switch (state) {
            case 0: //vychozi stav
                printf("DEBUG: begin | znak: %c\n", symbol);
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
                printf("DEBUG: komentar | znak: %c\n", symbol);
                if (symbol == '\n'){
                    state = 0;
                }else{
                    state = 1;
                }
                break;
            case 2: //'='
                printf("DEBUG: '='| znak: %c\n", symbol);
                if (symbol == 'b'){ //moznost viceradkoveho komentare
                    stringAddChar(&str, symbol);
                    cmnt_begin = 1;
                    state = 5;
                }else if (symbol == '='){
                    token.type = EQ_REL;
                    token.attr = stringGet(&str);
                    return token;
                }else{
                    token.type = ASSIGNMENT;
                    token.attr = "";
                    return token;
                }
                break;
            case 3: //=begin TODO
                if (symbol == '='){
                    stringClear(&str);
                    state = 4;
                }else{
                    state = 3;
                }
                break;

            case 4: //=
            printf("DEBUG: Koment: Uz mozna koncim! \n");
                stringAddChar(&str, symbol);
                if (str->length < 3){
                    state = 4;
                }else{
                    if (stringCompreConst(&str, "end")){
                        state = 0;
                    }else{
                        state = 3;
                    }
                }
                break;
            //TODO END

            case 5: //identifikator / klicove slovo
                printf("DEBUG: 'ID/KW'| znak: %c | buffer: %s\n ", symbol, stringGet(&str));
                if (isalnum(symbol) || symbol == '_'){
                    stringAddChar(&str, symbol);
                    state = 5;
                }else if (symbol == '?' || symbol == '!'){
                    stringAddChar(&str, symbol);
                    state = 6;
                }
                else if (stringCompareConst(&str, "def")){ token.type = KW; token.attr = "def"; return token;}
                else if (stringCompareConst(&str, "do")){ token.type = KW; token.attr = "do"; return token;}
                else if (stringCompareConst(&str, "else")){ token.type = KW; token.attr = "else"; return token;}
                else if (stringCompareConst(&str, "end")){ token.type = KW; token.attr = "end"; return token;}
                else if (stringCompareConst(&str, "if")){ token.type = KW; token.attr = "if"; return token;}
                else if (stringCompareConst(&str, "not")){ token.type = KW; token.attr = "not"; return token;}
                else if (stringCompareConst(&str, "nil")){ token.type = KW; token.attr = "nil"; return token;}
                else if (stringCompareConst(&str, "then")){ token.type = KW; token.attr = "then"; return token;}
                else if (stringCompareConst(&str, "while")){ token.type = KW; token.attr = "while"; return token;}
                }else if (stringCompareConst(&str, "begin")){
                    if(cmnt_begin == 1){
                        state = 3;
                    }else{
                        printf("DEBUG: Koment: Zkurvilo se to, nejsu begin, vracam 5 uvidzime co dal\n");
                        ungetc(stdin, 5);
                        token.type = EQ;
                        token.attr = "";
                        return token;
                    }
                }else{
                    token.type = ID;
                    token.attr = stringGet(&str);
                    return token;
                }
                break;
            case 6: //indetifikator + ? nebo ! (mozny konec identifikatoru)



                break;
        }
    }

    stringFree(&str);
}
int main(){
    t_Token ret_token = getNextToken();
    printf("DEBUG: Token: %d | Attr: %s\n", ret_token.type, ret_token.attr);
    return 0;
}
