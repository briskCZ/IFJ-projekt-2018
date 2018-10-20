#include "scanner.h"
#include "ret_vals.h"

//DEBUG FUNKCe
int main(){
    t_Token ret_token;
    //pridani znaku noveho radku, na zacatek souboru, kvuli blokovym komentarum
    ungetc('\n', stdin); // !!! pro funkcnost blokoveho komentare
    scannerInit();
    do {
        int error;
        ret_token = getNextToken(&error);
        printToken(ret_token, error);
    } while (ret_token.type != T_EOF);
    scannerClean();
    return 0;
}
