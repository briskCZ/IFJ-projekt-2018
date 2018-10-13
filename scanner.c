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

t_Token token;  //token, ktery bude vracen

void printToken(t_Token t, int error){
    char * type;
    switch (t.type){
        case NIL: type="NIL"; break;
        case EQ_REL: type="EQ_REL"; break;
        case ASSIGNMENT: type = "ASSIGNMENT"; break;
        case MUL: type = "MUL"; break;
        case DIV: type = "DIV"; break;
        case PLUS: type = "PLUS"; break;
        case MINUS: type = "MINUS"; break;
        case LEFT_PAR: type = "LEFT_PAR"; break;
        case RIGHT_PAR: type = "RIGHT_PAR"; break;
        case LESS: type = "LESS"; break;
        case MORE: type = "MORE"; break;
        case LESS_EQ: type = "LESS_EQ"; break;
        case MORE_EQ: type = "MORE_EQ"; break;
        case NOT_EQ: type = "NOT_EQ"; break;
        case STR: type = "STR"; break;
        case COMMA: type = "COMMA"; break;
        case DEF: type = "DEF"; break;
        case DO: type = "DO"; break;
        case ELSE: type = "ELSE"; break;
        case END: type = "END"; break;
        case IF: type = "IF"; break;
        case NOT: type = "NOT"; break;
        case THEN: type = "THEN"; break;
        case WHILE: type = "WHILE"; break;
        case T_EOF: type = "EOF"; break;
        case ID: type = "ID"; break;
        case INT: type = "INT"; break;
        case DOUBLE: type = "DOUBLE"; break;
        case STRING: type = "STRING"; break;
    }
    printf("TOKEN: %s | Attr: %s | Error: %d \n", type, stringGet(&t.attr), error);
}

t_Token getNextToken(int *error){
    *error = SUCCESS;
    string buffer;     //buffer pro identifikatory a kw
    stringInit(&buffer);
    stringClear(&token.attr);
    int string_hex_count = 0;
    int state = 0;
    int digit_zc = 0; //pocet nul v cisle

    while(42){
        //printf("DEBUG: State: %d\n", state);
        char symbol = getc(stdin);
        if (symbol == EOF){
            token.type = T_EOF;
            return token;
        }
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
                    strAdc(&buffer, symbol);
                    state = S_ID_KW;
                }else if (isdigit(symbol)){
                    if(symbol == '0') digit_zc++;
                    strAdc(&buffer, symbol);
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
                    token.type = MUL; stringClear(&token.attr);
                    return token;
                }else if (symbol == '/'){
                    token.type = DIV; stringClear(&token.attr);
                    return token;
                }else if (symbol == '+'){
                    token.type = PLUS; stringClear(&token.attr);
                    return token;
                }else if (symbol == '-'){
                    token.type = MINUS; stringClear(&token.attr);
                    return token;
                }else if (symbol == '('){
                    token.type = LEFT_PAR; stringClear(&token.attr);
                    return token;
                }else if (symbol == ')'){
                    token.type = RIGHT_PAR; stringClear(&token.attr);
                    return token;
                }else if (symbol == ','){
                    token.type = COMMA; stringClear(&token.attr);
                }else{
                    *error = ERROR_LEX;
                    stringClear(&buffer);
                    fprintf(stderr, "ERROR_LEX: Unexpected symbol\n");
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
                    strAdc(&buffer, symbol);
                    state = S_BC_BEGIN;
                }else if (symbol == '='){
                    token.type = EQ_REL; stringCopy(&token.attr, &buffer);
                    state = S_START;
                    return token;
                }else{
                    ungetc(symbol, stdin);
                    token.type = ASSIGNMENT; stringClear(&token.attr);
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
                    strAdc(&buffer, symbol);
                    //stringPrint(&buffer);
                    state = S_ID_KW;
                    if (stringCompareConst(&buffer, "def") == 0){ token.type = DEF; return token;}
                    else if (stringCompareConst(&buffer, "do") == 0){ token.type = DO; return token;}
                    else if (stringCompareConst(&buffer, "else") == 0){ token.type = ELSE; return token;}
                    else if (stringCompareConst(&buffer, "end") == 0){ token.type = END; return token;}
                    else if (stringCompareConst(&buffer, "if") == 0){ token.type = IF; return token;}
                    else if (stringCompareConst(&buffer, "not") == 0){ token.type = NOT; return token;}
                    else if (stringCompareConst(&buffer, "nil") == 0){ token.type = NIL; return token;}
                    else if (stringCompareConst(&buffer, "then") == 0){ token.type = THEN; return token;}
                    else if (stringCompareConst(&buffer, "while") == 0){ token.type = WHILE; return token;}
                }else if (symbol == '?' || symbol == '!'){
                    strAdc(&buffer, symbol);
                    token.type = ID; stringCopy(&token.attr, &buffer);
                    return token;
                }else{ //TODO mozna tu maji byt specialni znaky, ne cokoliv
                    ungetc(symbol, stdin);
                    token.type = ID; stringCopy(&token.attr, &buffer);
                    return token;
                }
                break;

            case S_LESS:    // <, <=
                stringClear(&token.attr);
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
                stringClear(&token.attr);
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
                    stringClear(&token.attr);
                    return token;
                }else{
                    fprintf(stderr, "LEX_ERR\n");
                    stringClear(&buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                }
                break;

            case S_STRING: //"string"
                if (symbol == '"'){
                    token.type = STR; stringCopy(&token.attr, &buffer);
                    return token;
                }else if (symbol > 31 && symbol != '\\'){ //znaky vetsi nez ascii 31
                    strAdc(&buffer, symbol);
                    state = S_STRING;
                }else if (symbol == '\\'){
                    //specialni znak
                    strAdc(&buffer, symbol);
                    state = S_SPECIAL_SYMBOL;
                    string_hex_count = 0;
                }else{
                    fprintf(stderr, "ERROR_LEX: Invalid symbols in string\n");
                    stringClear(&buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                }
                break;

            case S_SPECIAL_SYMBOL: //
                if (symbol == '"' || symbol == 'n' || symbol == 't' || symbol == 's' || symbol == '\\'){
                    strAdc(&buffer, symbol);
                    state = S_STRING;
                }else if (symbol == 'x'){
                    strAdc(&buffer, symbol);
                    state = S_SPECIAL_HEX;
                }else{
                    /*fprintf(stderr, "ERROR_LEX: Invalid escape sequence symbol\n");
                    *error = ERROR_LEX; ==podle fora ok*/
                    state = S_STRING;
                }
                break;

            case S_SPECIAL_HEX: //\xhh
                if (isValidHex(symbol) && string_hex_count <= 2){ //max hex
                    string_hex_count++;
                    strAdc(&buffer, symbol);
                    if (string_hex_count == 2){
                        state = S_STRING;
                    }else{
                        state = S_SPECIAL_HEX;
                    }
                }else{
                    //pokud je hexa pouze jeden symbol
                    if (!isValidHex(symbol) && string_hex_count == 1){
                        state = S_STRING;
                        ungetc(symbol, stdin);
                    }else{
                        fprintf(stderr, "ERROR_LEX: Invalid hexadecimal\n");
                        stringClear(&buffer);
                        *error = ERROR_LEX;
                        state = S_START;
                    }
                }
                break;

            case S_DIGIT: //vychozi stav pro cisla (kvuli poctu nul)
                if (symbol == '0'){
                    strAdc(&buffer, symbol);
                    digit_zc++;
                }
                if (isdigit(symbol) && symbol != '0' && digit_zc == 0){
                    strAdc(&buffer, symbol);
                    state = S_INT;
                }else if (isNumberEnding(symbol) && digit_zc <= 1){
                    token.type = INT; stringCopy(&token.attr, &buffer);
                    return token;
                }else if (isNumberEnding(symbol) && digit_zc > 1){  //maximum nul pred cislem
                    fprintf(stderr, "ERROR_LEX: Too many 0s in whole number part\n");
                    stringClear(&buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                    digit_zc = 0;
                }

                break;

            case S_INT:
                if (isdigit(symbol)){
                    strAdc(&buffer, symbol);
                    state = S_INT;
                }else if (symbol ==  'e' || symbol == 'E'){
                    strAdc(&buffer, symbol);
                    state = S_EXPONENT;
                }else if (symbol == '.'){
                    strAdc(&buffer, symbol);
                    state = S_DOUBLE;
                }else if (isNumberEnding(symbol)){
                    if(strtol(stringGet(&buffer), NULL, 10) <= INT_MAX){
                        token.type = INT; stringCopy(&token.attr, &buffer);
                        return token;
                    }else{
                        fprintf(stderr, "ERROR_LEX: Number limit!\n");
                        stringClear(&buffer);
                        *error = ERROR_LEX;
                        state = S_START;
                    }
                }else{
                    fprintf(stderr, "ERROR_LEX: Wrong number format!\n");
                    stringClear(&buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                }
                break;

            case S_EXPONENT:
                break;
            case S_DOUBLE:
                break;
        }
    }

    stringFree(&buffer);
}

int isValidHex(char c){
    //if ((C >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))
    if ((c >= 47 && c <= 57) || (c >= 65 && c <= 70)){
        return 1;
    }else{
        return 0;
    }
}
int isNumberEnding(char c){
    if (c == '+' || c == '-' || c == '*' || c == ')' || c == '='
        || c == '<' || c == '>' || c == '!' || c == '\n' || c == ' '){
        return 1;
    }else{
        return 0;
    }
}

int scannerInit(){
    return stringInit(&token.attr);
}
void scannerClean(){
    stringFree(&token.attr);
}
