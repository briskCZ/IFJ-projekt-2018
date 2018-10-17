/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Implementace scanneru
 *  Pozn. u funkci pro praci se stringem se pomoci makra kontroluji chyby.
*/

#include "scanner.h"

t_Token sc_token;  //token, ktery bude vracen
string sc_buffer;  //buffer pro identifikatory a kw
string sc_aux_buffer; //pomocny buffer pro blokove komentare a hexa cisla
int sc_uab;

t_Token getNextToken(int *error){
    *error = SUCCESS;
    stringClear(&sc_buffer);
    stringClear(&sc_token.attr);

    char symbol;
    int ab_index = 0;           //index v pomocnem bufferu
    int double_sad = 0;         //symbolu zateckou v double
    int string_hex_count = 0;   //pocet cisel za x
    int state = 0;              //stav automatu
    int digit_zc = 0;           //pocet nul v cisle
    int exponent_sign = 0;      //pomocna promena pro znamenko,
    int digits_ae = 0;          //cisla po exponentu
    int was_eq = 0;             // 0 - byl komentar, 1 bylo =
                                //pokud je desetine cislo v exponencialnim formatu
    while(42){
        printf("DEBUG: S: %d | UAB: %d | AB: %s\n", state, sc_uab, stringGet(&sc_aux_buffer));
        // Rozhodnuti ktery buffer pouzivat
        if(sc_uab == 0){
            symbol = getc(stdin);
        }else{
            symbol = sc_aux_buffer.val[ab_index];
            //printf("DEBUG: Symbol: %c\n", symbol);
            ab_index++;
            // Pokud jiz nejsou dalsi dostupne znaky v aux bufferu
            if (ab_index == stringGetLength(&sc_aux_buffer)){
                sc_uab = 0;
                ab_index = 0;
                stringClear(&sc_aux_buffer);
            }
        }
        if (symbol == EOF){
            sc_token.type = T_EOF;
            return sc_token;
        }
        switch (state) {
            case S_START: //vychozi stav, pokud nacte lexem s delkou jedna, vrati ho
                ////printf("DEBUG: begin | znak: %c\n", symbol);
                if (isspace(symbol)){
                    state = S_START;
                }else if (symbol == '#'){ //
                    state = S_LINE_COMMENT;
                }else if (symbol == '='){
                    state = S_EQUALS;
                }else if (symbol == '_' || islower(symbol)){
                    strAdc(&sc_buffer, symbol);
                    state = S_ID_KW;
                }else if (isdigit(symbol)){
                    if(symbol == '0') digit_zc++;
                    strAdc(&sc_buffer, symbol);
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
                    sc_token.type = MUL; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == '/'){
                    sc_token.type = DIV; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == '+'){
                    sc_token.type = PLUS; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == '-'){
                    sc_token.type = MINUS; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == '('){
                    sc_token.type = LEFT_PAR; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == ')'){
                    sc_token.type = RIGHT_PAR; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == ','){
                    sc_token.type = COMMA; stringClear(&sc_token.attr);
                }else{
                    *error = ERROR_LEX;
                    stringClear(&sc_buffer);
                    fprintf(stderr, "ERROR_LEX: Unexpected symbol\n");
                }
                break;
            case S_LINE_COMMENT: // #radkovy komentar
                ////printf("DEBUG: komentar | znak: %c\n", symbol);
                if (symbol == '\n'){
                    state = S_START;
                }else{
                    state = S_LINE_COMMENT;
                }
                break;

            case S_EQUALS: // =, =b, ==
                ////printf("DEBUG: '='| znak: %c\n", symbol);
                /*if (symbol == 'b'){ //moznost viceradkoveho komentare
                    strAdc(&sc_aux_buffer, symbol);
                    cmnt_begin_cc++;
                    state = S_BC_BEGIN;
                }*/
                if (was_eq == 0 && isCmntBegin(symbol, &was_eq)){
                    state = S_BLOCK_COMMENT;
                }else if (symbol == '='){
                    sc_token.type = EQ_REL; strCopy(&sc_token.attr, &sc_buffer);
                    state = S_START;
                    return sc_token;
                }else{
                    ungetc(symbol, stdin);
                    sc_token.type = ASSIGNMENT; stringClear(&sc_token.attr);
                    state = S_START;
                    return sc_token;
                }
                break;

            case S_BLOCK_COMMENT: //zustan dokud nenarazis na end
                if (symbol == '=' && isCmntEnd()){
                    state = S_START;
                }else{
                    state = S_BLOCK_COMMENT;
                }
                break;

            // case S_BC_END:
            //     if (isCmntEnd()){
            //         state = S_START;
            //     }else{
            //         state = S_BC_END;
            //     }
            //     break;

            case S_ID_KW: //identifikator / klicove slovo
                if (isalnum(symbol) || symbol == '_'){
                    strAdc(&sc_buffer, symbol);
                    //DEBUGstringPrint(&sc_buffer);
                    state = S_ID_KW;
                    if (stringCompareConst(&sc_buffer, "def") == 0){ sc_token.type = DEF; return sc_token;}
                    else if (stringCompareConst(&sc_buffer, "do") == 0){ sc_token.type = DO; return sc_token;}
                    else if (stringCompareConst(&sc_buffer, "else") == 0){ sc_token.type = ELSE; return sc_token;}
                    else if (stringCompareConst(&sc_buffer, "end") == 0){ sc_token.type = END; return sc_token;}
                    else if (stringCompareConst(&sc_buffer, "if") == 0){ sc_token.type = IF; return sc_token;}
                    else if (stringCompareConst(&sc_buffer, "not") == 0){ sc_token.type = NOT; return sc_token;}
                    else if (stringCompareConst(&sc_buffer, "nil") == 0){ sc_token.type = NIL; return sc_token;}
                    else if (stringCompareConst(&sc_buffer, "then") == 0){ sc_token.type = THEN; return sc_token;}
                    else if (stringCompareConst(&sc_buffer, "while") == 0){ sc_token.type = WHILE; return sc_token;}
                }else if (symbol == '?' || symbol == '!'){
                    strAdc(&sc_buffer, symbol);
                    sc_token.type = ID; strCopy(&sc_token.attr, &sc_buffer);
                    return sc_token;
                }else{ //TODO mozna tu maji byt specialni znaky, ne cokoliv
                    ungetc(symbol, stdin);
                    sc_token.type = ID; strCopy(&sc_token.attr, &sc_buffer);
                    return sc_token;
                }
                break;

            case S_LESS:    // <, <=
                stringClear(&sc_token.attr);
                if (symbol == '='){
                    sc_token.type = LESS_EQ;
                    return sc_token;
                }else{
                    sc_token.type = LESS;
                    ungetc(symbol, stdin);
                    return sc_token;
                }
                break;

            case S_MORE:    // >, >=
                stringClear(&sc_token.attr);
                if (symbol == '='){
                    sc_token.type = MORE_EQ;
                    return sc_token;
                }else{
                    sc_token.type = MORE;
                    ungetc(symbol, stdin);
                    return sc_token;
                }
                break;

            case S_NOT_EQ: //!=
                if (symbol == '='){
                    sc_token.type = NOT_EQ;
                    stringClear(&sc_token.attr);
                    return sc_token;
                }else{
                    fprintf(stderr, "LEX_ERR\n");
                    stringClear(&sc_buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                }
                break;

            case S_STRING: //"string"
                if (symbol == '"'){
                    sc_token.type = STR; strCopy(&sc_token.attr, &sc_buffer);
                    return sc_token;
                }else if (symbol > 31 && symbol != '\\'){ //znaky vetsi nez ascii 31
                    strAdc(&sc_buffer, symbol);
                    state = S_STRING;
                }else if (symbol == '\\'){
                    //specialni znak
                    strAdc(&sc_buffer, symbol);
                    state = S_SPECIAL_SYMBOL;
                    string_hex_count = 0;
                }else{
                    fprintf(stderr, "ERROR_LEX: Invalid symbols in string\n");
                    stringClear(&sc_buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                }
                break;

            case S_SPECIAL_SYMBOL: //
                if (symbol == '"' || symbol == 'n' || symbol == 't' || symbol == 's' || symbol == '\\'){
                    strAdc(&sc_buffer, symbol);
                    state = S_STRING;
                }else if (symbol == 'x'){
                    string_hex_count = 0;
                    stringClear(&sc_aux_buffer);
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
                    strAdc(&sc_aux_buffer, symbol);
                    // strAdc(&sc_buffer, symbol);
                    if (string_hex_count == 2){
                        sc_buffer.val[sc_buffer.length-1] = hexToChar(&sc_aux_buffer);
                        state = S_STRING;
                    }else{
                        state = S_SPECIAL_HEX;
                    }
                }else{
                    //pokud je hexa pouze jeden symbol
                    if (!isValidHex(symbol) && string_hex_count == 1){
                        sc_buffer.val[sc_buffer.length-1] = hexToChar(&sc_aux_buffer);
                        state = S_STRING;
                        ungetc(symbol, stdin);
                    }else{
                        fprintf(stderr, "ERROR_LEX: Invalid hexadecimal\n");
                        stringClear(&sc_buffer);
                        *error = ERROR_LEX;
                        state = S_START;
                    }
                }
                break;

            case S_DIGIT: //vychozi stav pro cisla (kvuli poctu nul)
                ////printf("DEBUG: WNZ %d | DZC %d | SYM %d \n", was_nonzero, digit_zc, symbol);
                if (symbol == '0')
                    digit_zc++;
                if ((isdigit(symbol) && symbol != '0' && digit_zc == 0) || (isdigit(symbol) && sc_buffer.val[0] != '0')){
                    strAdc(&sc_buffer, symbol);
                    state = S_INT;
                }else if (isNumberEnding(symbol) && digit_zc <= 1){
                    sc_token.type = INT; strCopy(&sc_token.attr, &sc_buffer);
                    return sc_token;
                }else if (isNumberEnding(symbol) && digit_zc > 1){  //maximum nul pred cislem
                    fprintf(stderr, "ERROR_LEX: Too many 0s in whole number part\n");
                    stringClear(&sc_buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                    digit_zc = 0;
                //naschval tady
                }else if (symbol == '.' && digit_zc <= 1){
                    strAdc(&sc_buffer, symbol);
                    state = S_DOUBLE;
                }else if ((symbol == 'e' || symbol == 'E') && digit_zc <= 1){
                    strAdc(&sc_buffer, symbol);
                    state = S_EXPONENT;
                }
                break;

            case S_INT:
                if (isdigit(symbol)){
                    strAdc(&sc_buffer, symbol);
                    state = S_INT;
                }else if (symbol ==  'e' || symbol == 'E'){
                    strAdc(&sc_buffer, symbol);
                    state = S_EXPONENT;
                }else if (symbol == '.'){
                    strAdc(&sc_buffer, symbol);
                    state = S_DOUBLE;
                    double_sad = 0;
                }else if (isNumberEnding(symbol)){
                    if(strtol(stringGet(&sc_buffer), NULL, 10) <= INT_MAX){
                        sc_token.type = INT; strCopy(&sc_token.attr, &sc_buffer);
                        return sc_token;
                    }else{
                        fprintf(stderr, "ERROR_LEX: Number limit!\n");
                        stringClear(&sc_buffer);
                        *error = ERROR_LEX;
                        state = S_START;
                        double_sad = 0;
                        digit_zc = 0;
                    }
                }else{
                    fprintf(stderr, "ERROR_LEX: Wrong number format!\n");
                    stringClear(&sc_buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                    double_sad = 0;
                    digit_zc = 0;

                }
                break;

            case S_EXPONENT:
                if ((symbol == '+' || symbol == '-') && exponent_sign == 0){
                    exponent_sign = 1;
                    strAdc(&sc_buffer, symbol);
                }else if (exponent_sign == 1){
                    //je to ok, za exponentem je jeste nejake cislo
                    if (isdigit(symbol)){
                        digits_ae++;
                        strAdc(&sc_buffer, symbol);
                        //aby se do teto vetve nedostal znovu
                        exponent_sign++;
                    }else{
                        fprintf(stderr, "ERROR_LEX: Wrong exponent format!\n");
                        stringClear(&sc_buffer);
                        *error = ERROR_LEX;
                        state = S_START;
                        exponent_sign = 0;
                        digits_ae = 0;
                        digit_zc = 0;


                    }
                }else if (isdigit(symbol)){
                    digits_ae++;
                    strAdc(&sc_buffer, symbol);
                    state = S_EXPONENT;
                }else if (isNumberEnding(symbol) && digits_ae >= 1  ){
                    sc_token.type = DOUBLE;
                    strCopy(&sc_token.attr, &sc_buffer);
                    return sc_token;
                }else{
                    fprintf(stderr, "ERROR_LEX: Wrong exponent format!\n");
                    stringClear(&sc_buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                    exponent_sign = 0;
                    digits_ae = 0;
                    digit_zc = 0;

                }
                break;

            case S_DOUBLE:
                if (isdigit(symbol)){
                    strAdc(&sc_buffer, symbol);
                    double_sad++;
                }else if ((symbol == 'e' || symbol == 'E') && double_sad > 0){
                    strAdc(&sc_buffer, symbol);
                    state = S_EXPONENT;
                }else if (isNumberEnding(symbol) && double_sad >= 1){
                    sc_token.type = DOUBLE; strCopy(&sc_token.attr, &sc_buffer);
                    return sc_token;
                }else{
                    fprintf(stderr, "ERROR_LEX: Wrong double format!\n");
                    stringClear(&sc_buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                    double_sad = 0;
                    digit_zc = 0;


                }
                break;
        }
    }

}

int isValidHex(char c){
    //if ((C >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))
    if ((c >= 47 && c <= 57) || (c >= 65 && c <= 70)){
        return 1;
    }else{
        return 0;
    }
}

int hexToChar(string *s){
    return (char)strtol(stringGet(s), NULL, 16); //16tkova soustava
}

int isNumberEnding(char c){
    if (c == '+' || c == '-' || c == '*' || c == ')' || c == '='
        || c == '<' || c == '>' || c == '!' || c == '\n' || c == ' '){
        ungetc(c, stdin);
        return 1;
    }else{
        return 0;
    }
}

int scannerInit(){
    int ret_val = stringInit(&sc_buffer);
    ret_val += stringInit(&sc_token.attr);
    ret_val += stringInit(&sc_aux_buffer);
    return (ret_val == 0) ? SUCCESS : ERROR_INTERNAL;
}

void scannerClean(){
    stringFree(&sc_token.attr);
    stringFree(&sc_buffer);
    stringFree(&sc_aux_buffer);
}
int isCmntEnd(){
    char *cmnt_end = "end";
    int cmnt_end_length = 3;
    char c;

    for (int i = 0; i < cmnt_end_length; i++){
        c = getc(stdin);
        if (c != cmnt_end[i]){
            if (c == '='){
                ungetc(c, stdin);
            }
            return 0;
        }
    }
    return 1;

    stringClear(&sc_aux_buffer);
    return 0;
}

int isCmntBegin(char symbol, int *was_eq){
    ungetc(symbol, stdin);
    char *cmnt_start = "begin";
    int cmnt_start_length = 5;
    char c;
    stringClear(&sc_aux_buffer);
    for (int i = 0; i < cmnt_start_length; i++){
        if (i > 0) sc_uab = 1;
        c = getc(stdin);
        stringAddChar(&sc_aux_buffer, c);
        if (c != cmnt_start[i]){
            *was_eq = 1;
            return 0;
        }
    }
    return 1;

}
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
