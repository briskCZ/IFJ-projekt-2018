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

t_Token getNextToken(int *error){
    if (sc_using_mem_token == 1){
        // printf("TOKEN!!!! POZOR\n");
        // printToken(sc_mem_token, 0);
        // printf("KONECTOKEN\n");
        sc_using_mem_token = 0;
        return sc_mem_token;
    }
    *error = SUCCESS;
    stringClear(&sc_buffer);
    stringClear(&sc_token.attr);
    char symbol = 0;
    int double_sad = 0;         //symbolu zateckou v double
    int exp_sign = 0;           //urcuje zda bylo znamenko v exponentu
    int exp_nc = 0;             //pocet cisel v exponentu
    int string_hex_count = 0;   //pocet cisel za x

    int state = S_START;        //stav automatu

    while(42){
        // Rozhodnuti ktery buffer pouzivat
        if(sc_uab == 0){
            symbol = getc(stdin);
        }else{
            symbol = sc_aux_buffer.val[sc_abi];
            sc_abi++;
            // Pokud jiz nejsou dalsi dostupne znaky v aux bufferu
            if (sc_abi == stringGetLength(&sc_aux_buffer)){
                sc_uab = 0;
                sc_abi = 0;
                stringClear(&sc_aux_buffer);
            }
        }
        //fprintf(stderr, "Symbol: %d line: %d\n", symbol, sc_line_cnt);
        // fprintf(stderr,"DEBUG: S: %d |SYM: %d %c| UAB: %d | AB: %s | ABI: %d| B: %s | LC: %d\n", state, symbol, symbol, sc_uab, stringGet(&sc_aux_buffer), sc_abi, stringGet(&sc_buffer), sc_line_cnt);
        if (symbol == EOF){
            sc_token.type = T_EOF;
            return sc_token;
        }
        switch (state) {
            case S_START: //vychozi stav, pokud nacte lexem s delkou jedna, vrati hoe
                if (isspace(symbol)){
                    if (symbol == '\n'){
                        sc_line_cnt++;
                        //fprintf(stderr,"Line: %d\n", sc_line_cnt);
                        sc_was_eol = 1;
                        sc_token.type = T_EOL;
                        return sc_token;
                    }else{
                        sc_was_eol = 0;
                        state = S_START;
                    }
                }else if (symbol == '#'){ //
                    state = S_LINE_COMMENT;
                }else if (symbol == '='){
                    if ((sc_line_cnt == 1 || sc_was_eol == 1) && isCmntBegin(symbol)){
                        sc_line_cnt++;
                        state = S_BLOCK_COMMENT;
                    }else{
                        state = S_EQUALS;
                    }
                }else if (symbol == '_' || islower(symbol)){
                    strAdc(&sc_buffer, symbol);
                    state = S_ID_KW;
                }else if (isdigit(symbol)){
                    strAdc(&sc_buffer, symbol);
                    if(symbol == '0'){
                        state = S_ZERO;
                    }else{
                        state = S_INT;
                    }
                }else if (symbol == '<'){
                    state = S_LESS;
                }else if (symbol == '>'){
                    state = S_MORE;
                }else if (symbol == '!'){
                    state = S_NOT_EQ;
                }else if (symbol == '"'){
                    state = S_STRING;
                }else if (symbol == '*'){
                    sc_token.type = T_MUL; sc_was_eol = 0; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == '/'){
                    sc_token.type = T_DIV; sc_was_eol = 0; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == '+'){
                    sc_token.type = T_PLUS; sc_was_eol = 0; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == '-'){
                    sc_token.type = T_MINUS; sc_was_eol = 0; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == '('){
                    sc_token.type = T_LEFT_PAR; sc_was_eol = 0; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == ')'){
                    sc_token.type = T_RIGHT_PAR; sc_was_eol = 0; stringClear(&sc_token.attr);
                    return sc_token;
                }else if (symbol == ','){
                    sc_token.type = T_COMMA; sc_was_eol = 0; stringClear(&sc_token.attr);
                    return sc_token;
                }else{
                    *error = ERROR_LEX;
                    stringClear(&sc_buffer);
                    fprintf(stderr, "ERROR_LEX: Unexpected symbol, line: %d.\n", sc_line_cnt);
                    return sc_token;
                }
                break;
            case S_LINE_COMMENT: // #radkovy komentar
                //////printf("DEBUG: komentar | znak: %c\n", symbol);
                if (symbol == '\n'){
                    state = S_START;
                    sc_line_cnt++;
                    sc_token.type = T_EOL;
                    sc_was_eol = 1;
                    return sc_token;
                }else{
                    state = S_LINE_COMMENT;
                }
                break;

            case S_EQUALS: // =, ==
                if (symbol == '='){
                    sc_was_eol = 0;
                    sc_token.type = T_EQ_REL;
                    return sc_token;
                }else{
                    if (sc_uab){
                        stringUngetChar(symbol, &sc_aux_buffer);
                    }else{
                        ungetc(symbol, stdin);
                    }
                    sc_was_eol = 0;
                    sc_token.type = T_ASSIGNMENT;
                    return sc_token;
                }
                break;

            case S_BLOCK_COMMENT: //zustan dokud nenarazis na end
                //fprintf(stderr, "sc_eol : %d\n", sc_was_eol);
                if (sc_was_eol && symbol == '=' && isCmntEnd(&symbol)){
                    //pokud symbol po end byl \n
                    if (symbol == '\n'){
                        sc_line_cnt++;
                        sc_was_eol = 1;
                        state = S_START;
                    //pokud byl jiny whitespace znak, dokonci radek
                    }else{
                        state = S_BC_END;
                    }
                }else if (symbol == '\n'){
                    sc_was_eol = 1;
                    state = S_BLOCK_COMMENT;
                }else{
                    sc_was_eol = 0;
                    state = S_BLOCK_COMMENT;
                }
                break;

            case S_BC_END:  //do konce radku za =end
                if (symbol == '\n'){
                    //Vraceni EOL, ktery byl pred komentarem
                    sc_was_eol = 1;
                    sc_line_cnt++;
                    sc_token.type = T_EOL;
                    return sc_token;
                }else{
                    state = S_BC_END;
                }
                break;

            case S_ID_KW: //identifikator / klicove slovo
                if (isalnum(symbol) || symbol == '_'){
                    strAdc(&sc_buffer, symbol);
                    //DEBUGstringPrint(&sc_buffer);
                    sc_was_eol = 0;
                    state = S_ID_KW;
                    if (stringCompareConst(&sc_buffer, "def") == 0  && isKwEnd()){
                        sc_token.type = T_DEF;
                        return sc_token;
                    }else if (stringCompareConst(&sc_buffer, "do") == 0  && isKwEnd()){
                        sc_token.type = T_DO;
                        return sc_token;
                    }else if (stringCompareConst(&sc_buffer, "else") == 0  && isKwEnd()){
                        sc_token.type = T_ELSE;
                        return sc_token;
                    }else if (stringCompareConst(&sc_buffer, "end") == 0 && isKwEnd()){
                        sc_token.type = T_END;
                        return sc_token;
                    }else if (stringCompareConst(&sc_buffer, "if") == 0 && isKwEnd()){
                        sc_token.type = T_IF;
                        return sc_token;
                    }else if (stringCompareConst(&sc_buffer, "not") == 0 && isKwEnd()){
                        sc_token.type = T_NOT;
                        return sc_token;
                    }else if (stringCompareConst(&sc_buffer, "nil") == 0 && isKwEnd()){
                        sc_token.type = T_NIL;
                        return sc_token;
                    }else if (stringCompareConst(&sc_buffer, "then") == 0 && isKwEnd()){
                        sc_token.type = T_THEN;
                        return sc_token;
                    }else if (stringCompareConst(&sc_buffer, "while") == 0 && isKwEnd()){
                        sc_token.type = T_WHILE;
                        return sc_token;
                    }
                }else if (symbol == '?' || symbol == '!'){
                    strAdc(&sc_buffer, symbol);
                    sc_was_eol = 0;
                    sc_token.type = T_ID; strCopy(&sc_token.attr, &sc_buffer);
                    return sc_token;
                }else{
                    ungetc(symbol, stdin);
                    sc_was_eol = 0;
                    sc_token.type = T_ID; strCopy(&sc_token.attr, &sc_buffer);
                    return sc_token;
                }
                break;

            case S_LESS:    // <, <=
                stringClear(&sc_token.attr);
                sc_was_eol = 0;
                if (symbol == '='){
                    sc_token.type = T_LESS_EQ;
                    return sc_token;
                }else{
                    sc_token.type = T_LESS;
                    ungetc(symbol, stdin);
                    return sc_token;
                }
                break;

            case S_MORE:    // >, >=
                sc_was_eol = 0;
                stringClear(&sc_token.attr);
                if (symbol == '='){
                    sc_token.type = T_MORE_EQ;
                    return sc_token;
                }else{
                    sc_token.type = T_MORE;
                    ungetc(symbol, stdin);
                    return sc_token;
                }
                break;

            case S_NOT_EQ: //!=
                sc_was_eol = 0;
                if (symbol == '='){
                    sc_token.type = T_NOT_EQ;
                    stringClear(&sc_token.attr);
                    return sc_token;
                }else{
                    fprintf(stderr, "ERROR_LEX: ! must be followed by =, line: %d.\n", sc_line_cnt);
                    stringClear(&sc_buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                    return sc_token;
                }
                break;

            case S_STRING: //"string"
                sc_was_eol = 0;
                if (symbol == '"'){
                    stringToIns(&sc_buffer);
                    sc_token.type = T_STRING; strCopy(&sc_token.attr, &sc_buffer);
                    return sc_token;
                }else if (symbol == '\n'){
                    fprintf(stderr, "ERROR_LEX: String must be on one line, line: %d\n", sc_line_cnt);
                    *error = ERROR_LEX;
                    state = S_START;
                    return sc_token;
                }else if (symbol != 92){ //znaky vetsi nez ascii 31
                    strAdc(&sc_buffer, symbol);
                    state = S_STRING;
                }else if (symbol == 92){
                    //specialni znak
                    strAdc(&sc_buffer, symbol);
                    state = S_SPECIAL_SYMBOL;
                    string_hex_count = 0;
                }else{
                    fprintf(stderr, "ERROR_LEX: Invalid symbols in string, line: %d.\n", sc_line_cnt);
                    stringClear(&sc_buffer);
                    *error = ERROR_LEX;
                    state = S_START;
                    return sc_token;
                }
                break;

            case S_SPECIAL_SYMBOL: //
                sc_was_eol = 0;
                if (symbol == '"' || symbol == 'n' || symbol == 't' || symbol == 's' || symbol == '\\'){
                    strAdc(&sc_buffer, symbol);
                    state = S_STRING;
                }else if (symbol == 'x'){
                    string_hex_count = 0;
                    strAdc(&sc_buffer, symbol);
                    state = S_SPECIAL_HEX;
                }else{
                    /*fprintf(stderr, "ERROR_LEX: Invalid escape sequence symbol\n");
                    *error = ERROR_LEX; ==podle fora ok*/
                    state = S_STRING;
                    ungetc(symbol, stdin);
                }
                break;

            case S_SPECIAL_HEX: //\xhh
                sc_was_eol = 0;
                // fprintf(stderr, "symb: %d, %c | SHC: %d |  isVH %d\n", symbol, symbol, string_hex_count, isValidHex(symbol));
                if (isValidHex(symbol) && string_hex_count <= 2){ //max hex
                    string_hex_count++;
                    strAdc(&sc_buffer, symbol);
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
                        fprintf(stderr, "ERROR_LEX: Invalid hexadecimal, line: %d.\n", sc_line_cnt);
                        stringClear(&sc_buffer);
                        *error = ERROR_LEX;
                        state = S_START;
                        return sc_token;

                    }
                }
                break;

            case S_INT:
                sc_was_eol = 0;
                if (isdigit(symbol)){
                    strAdc(&sc_buffer, symbol);
                    state = S_INT;
                }else if (symbol == '.'){
                    strAdc(&sc_buffer, symbol);
                    state = S_FLOAT;
                }else if (symbol == 'e' || symbol == 'E'){
                    strAdc(&sc_buffer, symbol);
                    state = S_EXPONENT;
                }else if (isNumberEnding(symbol)){
                    sc_token.type = T_INT; strCopy(&sc_token.attr, &sc_buffer);
                    return sc_token;
                }else{
                    fprintf(stderr, "ERROR_LEX: Wrong int format on line %d\n", sc_line_cnt);
                    *error = ERROR_LEX;
                    return sc_token;
                }
                break;

            case S_ZERO:
                if (symbol >= '0' && symbol <= '7'){
                    strAdc(&sc_buffer, symbol);
                    state = S_OCTAL;
                }else if (isNumberEnding(symbol)){
                    sc_token.type = T_INT; strCopy(&sc_token.attr, &sc_buffer);
                    return sc_token;
                }else if (symbol == 'b'){
                    //strAdc(&sc_buffer, symbol);
                    state = S_BIN;
                }else if (symbol == 'x'){
                    //strAdc(&sc_buffer, symbol);
                    state = S_HEX;
                }else if (symbol == 'e' || symbol == 'E'){
                    strAdc(&sc_buffer, symbol);
                    state = S_EXPONENT;
                }else if (symbol == '.'){
                    strAdc(&sc_buffer, symbol);
                    state = S_FLOAT;
                }else{
                    fprintf(stderr, "ERROR_LEX: Wrong symbol after 0 on line: %d\n", sc_line_cnt);
                    *error = ERROR_LEX;
                    return sc_token;
                }
                break;

            case S_FLOAT:
                if (isdigit(symbol)){
                    strAdc(&sc_buffer, symbol);
                    double_sad++;
                }else if (isNumberEnding(symbol)){
                    if (double_sad == 0){
                        fprintf(stderr, "ERROR_LEX: Zero numbers after . on_line: %d\n", sc_line_cnt);
                        *error = ERROR_LEX;
                        return sc_token;
                    }else{
                        sc_token.type = T_FLOAT;
						strCopy(&sc_token.attr, &sc_buffer);
                        return sc_token;
                    }
                }else if (symbol == 'e' || symbol == 'E'){
                    if (double_sad == 0){
                        fprintf(stderr, "ERROR_LEX: Zero numbers after . on_line: %d\n", sc_line_cnt);
                        *error = ERROR_LEX;
                        return sc_token;
                    }else{
                        strAdc(&sc_buffer, symbol);
                        state = S_EXPONENT;
                    }
                }else{
                    fprintf(stderr, "ERROR_LEX: Invalid symbol in float on line: %d\n", sc_line_cnt);
                    *error = ERROR_LEX;
                    return sc_token;
                }
                break;

            case S_EXPONENT:
                if (exp_nc == 0){
                    strAdc(&sc_buffer, symbol);
                    if ((symbol == '+' || symbol == '-') && exp_sign == 0){
                        exp_sign = 1;
                        state = S_EXPONENT;
                        //precteni dalsiho znaku kvuli pokracovani exponentu
                        symbol = getc(stdin);
                    }else if (!isdigit(symbol)){
                        fprintf(stderr, "ERROR_LEX: +, - or digit expected after exponent on line %d\n", sc_line_cnt);
                        *error = ERROR_LEX;
                        return sc_token;
                    }
                }
                if (isdigit(symbol)){
                    if (isdigit(symbol)){
                        exp_nc++;
                    }
                    strAdc(&sc_buffer, symbol);
                    state = S_EXPONENT;
                }else if (isNumberEnding(symbol)){
                    if (exp_nc > 0){
                        sc_token.type = T_FLOAT; strCopy(&sc_token.attr, &sc_buffer);
                        return sc_token;
                    }else{
                        fprintf(stderr, "ERROR_LEX: Literal expected after exponent on line: %d\n", sc_line_cnt);
                        *error = ERROR_LEX;
                        return sc_token;
                    }
                }else{
                    fprintf(stderr, "ERROR_LEX: Invalid symbol in exponent on line: %d\n", sc_line_cnt);
                    *error = ERROR_LEX;
                    return sc_token;
                }

                break;

            case S_OCTAL:
                if (stringGetLength(&sc_buffer) == 2 && symbol == '0'){
                    fprintf(stderr, "ERROR_LEX: Too many 0s in whole part on line: %d\n", sc_line_cnt);
                    *error = ERROR_LEX;
                    return sc_token;
                }else if (symbol >= '0' && symbol <= '7'){
                    strAdc(&sc_buffer, symbol);
                    state = S_OCTAL;
                }else if (isNumberEnding(symbol)){
                    /*TODO prevod do int */
                    char int_str[INT_LENGTH];
                    int value = strtol(stringGet(&sc_buffer), NULL, 8);
                    if (value == -1){
                        fprintf(stderr, "ERROR_LEX: Overflow of number on line: %d\n", sc_line_cnt);
                        *error = ERROR_LEX;
                        return sc_token;
                    }
                    sprintf(int_str, "%d", value);
                    stringInsert(&sc_token.attr, int_str);
                    sc_token.type = T_INT;
                    return sc_token;
                }else{
                    fprintf(stderr, "ERROR_LEX: Invalid symbol in octal on line: %d\n", sc_line_cnt);
                    *error = ERROR_LEX;
                    return sc_token;
                }
                break;

            case S_BIN:
                if (symbol == '0' || symbol == '1'){
                    strAdc(&sc_buffer, symbol);
                    state = S_BIN;
                }else if (isNumberEnding(symbol)){
                    //pouze 0b
                    if (stringGetLength(&sc_buffer) == 1){
                        fprintf(stderr, "ERROR_LEX: Binary literal without digits on line: %d\n", sc_line_cnt);
                        *error = ERROR_LEX;
                        return sc_token;
                    }else{
                        /*TODO prevod do int */
                        char int_str[INT_LENGTH];
                        int value = strtol(stringGet(&sc_buffer), NULL, 2);
                        if (value == -1){
                            fprintf(stderr, "ERROR_LEX: Overflow of number on line: %d\n", sc_line_cnt);
                            *error = ERROR_LEX;
                            return sc_token;
                        }
                        sprintf(int_str, "%d", value);
                        stringInsert(&sc_token.attr, int_str);
                        sc_token.type = T_INT;
                        return sc_token;
                    }
                }else{
                    fprintf(stderr, "ERROR_LEX: Invalid symbol in binary on line: %d\n", sc_line_cnt);
                    *error = ERROR_LEX;
                    return sc_token;
                }
                break;

            case S_HEX:
                if (isValidHex(symbol)){
                    strAdc(&sc_buffer, symbol);
                    state = S_HEX;
                }else if (isNumberEnding(symbol) && !isValidHex(symbol)){
                    if (stringGetLength(&sc_buffer) == 1){
                        fprintf(stderr, "ERROR_LEX: Hexadecimal literal without digits on line: %d\n", sc_line_cnt);
                        *error = ERROR_LEX;
                        return sc_token;
                    }else{
                        /*TODO prevod do int */
                        char int_str[INT_LENGTH];
                        int value = strtol(stringGet(&sc_buffer), NULL, 16);
                        if (value == -1){
                            fprintf(stderr, "ERROR_LEX: Overflow of number on line: %d\n", sc_line_cnt);
                            *error = ERROR_LEX;
                            return sc_token;
                        }
                        sprintf(int_str, "%d", value);
                        stringInsert(&sc_token.attr, int_str);
                        sc_token.type = T_INT;
                        return sc_token;
                    }
                }else{
                    fprintf(stderr, "ERROR_LEX: Invalid symbol in hexadecimal on line: %d\n", sc_line_cnt);
                    *error = ERROR_LEX;
                    return sc_token;
                }
                break;

        }
    }

}
void stringToIns(string *s){
    string in_s;
    stringInit(&in_s);
    stringCopy(&in_s, s);
    stringClear(s);
    int esc_len = 0;
    int length = stringGetLength(&in_s);
    for (int i = 0; i < length; i++){
        if (in_s.val[i] == 92){ // zpetne lomeno
            /* specialni znak */
            stringAddChar(s, 92);
            char nextChar;
            if (i + 1 < length){
                nextChar = in_s.val[i+1];
            }
            if (nextChar == 'x'){
                string tmp;
                stringInit(&tmp);
                if (i + 2 < length && isValidHex(in_s.val[i+2])){
                    esc_len = 1;
                    stringAddChar(&tmp, in_s.val[i+2]);
                    if (i + 3 < length && isValidHex(in_s.val[i+3])){
                        esc_len = 2;
                        stringAddChar(&tmp, in_s.val[i+3]);
                    }
                }
                sprintf(tmp.val, "%03ld", strtol(tmp.val, NULL, 16));
                //prekopirovani tmp na korektni pozici v output stringu
                for (unsigned int j = 0; j < strlen(tmp.val); j++){
                    stringAddChar(s, tmp.val[j]);
                }
                stringFree(&tmp);
                i += esc_len + 1; //zvys i o prectene znaky
            }else if(nextChar == 'n' ){
                stringAddChar(s, '0');
                stringAddChar(s, '1');
                stringAddChar(s, '0');
                i++;
            }else if(nextChar == 't' ){
                stringAddChar(s, '0');
                stringAddChar(s, '0');
                stringAddChar(s, '9');
                i++;
            }else if(nextChar == 's'){
                stringAddChar(s, '0');
                stringAddChar(s, '3');
                stringAddChar(s, '2');
                i++;
            }else if(nextChar == 92){
                stringAddChar(s, '0');
                stringAddChar(s, '9');
                stringAddChar(s, '2');
                i++;
            }else if(nextChar == '"'){
                stringAddChar(s, '0');
                stringAddChar(s, '3');
                stringAddChar(s, '4');
                i++;
            }else{
                //odstrani vlozene zpetne lomitko a necha znak znakem
                stringRemoveChar(s);
            }
        //prevod na nutne escape sekvence
        }else if ((in_s.val[i] >= 0 && in_s.val[i] <= 32) || in_s.val[i] == 35){
            char tmp[3];
            sprintf(tmp, "%03u", in_s.val[i]);
            //pridej \\xyz
            stringAddChar(s, 92);
            stringAddChar(s, tmp[0]);
            stringAddChar(s, tmp[1]);
            stringAddChar(s, tmp[2]);
        }else{
            //prevod vstupnich escape sekvenci
            stringAddChar(s, in_s.val[i]);
        }
    }
    stringFree(&in_s);
}

int isValidHex(char c){
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')){
        return 1;
    }else{
        return 0;
    }
}

int isNumberEnding(char c){
    if (c == '+' || c == '-' || c == '*' || c == ')' || c == '=' || c == '/'
        || c == '<' || c == '>' || c == '!' || c == '\n' || c == ' ' || c == ',' || c == '#'){
        ungetc(c, stdin);
        return 1;
    }else{
        return 0;
    }
}

int isKwEnd(){
    char symbol = getc(stdin);
    if (!isalnum(symbol) || isspace(symbol) || symbol == '#'){
        //pokud bude eol, tak ho musime taky vratit
        ungetc(symbol, stdin);
        return 1;
    }else{
        ungetc(symbol, stdin);
        return 0;
    }
}

int scannerInit(){
    sc_uab = 0;
    sc_abi = 0;
    sc_was_eol = 0;
    sc_line_cnt = 1;
    sc_using_mem_token = 0;
    int ret_val = stringInit(&sc_buffer);
    ret_val += stringInit(&sc_aux_buffer);
    ret_val += stringInit(&sc_mem_token.attr);
    ret_val += stringInit(&sc_token.attr);
    // ret_val += stringInit(&sc_token.attr);
    return (ret_val == 0) ? SUCCESS : ERROR_INTERNAL;
}

void scannerClean(){
    //stringFree(&sc_token.attr);
    stringFree(&sc_buffer);
    stringFree(&sc_aux_buffer);
    stringFree(&sc_mem_token.attr);
    stringFree(&sc_token.attr);
}

int isCmntEnd(char *sym){
    char *cmnt_end = "end";
    int cmnt_end_length = 3;
    char c;
    //kontrola posloupnosti znaku end
    for (int i = 0; i < cmnt_end_length; i++){
        c = getc(stdin);
        if (c != cmnt_end[i]){
            return 0;
        }
    }
    stringClear(&sc_aux_buffer);
    *sym = getc(stdin);
    return (isspace(*sym)) ? 1 : 0;
}

int isCmntBegin(char symbol){
    if (symbol != '=') return 0;
    char *cmnt_start = "=begin";
    int cmnt_start_length = 6;
    char c;

    //sc_uab = 1;
    //kontrola posloupnosti znaku =begin
    for (int i = 1; i < cmnt_start_length; i++){
        c = getc(stdin);
        stringAddChar(&sc_aux_buffer, c);
        //printf("DEBUG: C: %d | i: %d\n", c, i);
        if (c != cmnt_start[i]){

            sc_uab = 1;
            return 0;
        }
    }
    c = getc(stdin);
    if (isspace(c)){
        if (c == '\n') sc_line_cnt++;
        sc_uab = 0;
        stringClear(&sc_aux_buffer);
        return 1;
    }else{
        ungetc(c, stdin);
        sc_uab = 1;
        return 0;
    }
}
void printToken(t_Token t, int error){
    char * type;
    switch (t.type){
        case T_NIL: type="NIL"; break;
        case T_EQ_REL: type="EQ_REL"; break;
        case T_ASSIGNMENT: type = "ASSIGNMENT"; break;
        case T_MUL: type = "MUL"; break;
        case T_DIV: type = "DIV"; break;
        case T_PLUS: type = "PLUS"; break;
        case T_MINUS: type = "MINUS"; break;
        case T_LEFT_PAR: type = "LEFT_PAR"; break;
        case T_RIGHT_PAR: type = "RIGHT_PAR"; break;
        case T_LESS: type = "LESS"; break;
        case T_MORE: type = "MORE"; break;
        case T_LESS_EQ: type = "LESS_EQ"; break;
        case T_MORE_EQ: type = "MORE_EQ"; break;
        case T_NOT_EQ: type = "NOT_EQ"; break;
        case T_COMMA: type = "COMMA"; break;
        case T_DEF: type = "DEF"; break;
        case T_DO: type = "DO"; break;
        case T_ELSE: type = "ELSE"; break;
        case T_END: type = "END"; break;
        case T_IF: type = "IF"; break;
        case T_NOT: type = "NOT"; break;
        case T_THEN: type = "THEN"; break;
        case T_WHILE: type = "WHILE"; break;
        case T_EOF: type = "EOF"; break;
        case T_ID: type = "ID"; break;
        case T_INT: type = "INT"; break;
        case T_FLOAT: type = "FLOAT"; break;
        case T_STRING: type = "STRING"; break;
        case T_EOL: type = "EOL"; break;
    }
    fprintf(stderr,"TOKEN: %s | Attr: %s | Error: %d \n", type, stringGet(&t.attr), error);
}

int returnToken(t_Token token){
    if (sc_using_mem_token == 1){
        return MEMORY_ERROR;
    }
    sc_using_mem_token = 1;
    sc_mem_token = token;
    return SUCCESS;
}
t_Token getPrintNextToken(int *error){
    t_Token token = getNextToken(error);
    printToken(token, *error);
    return token;
}
