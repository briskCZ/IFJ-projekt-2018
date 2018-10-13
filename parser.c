#include "scanner.h"
#include "ret_vals.h"

int main(){
    while(1){
        int error;
        t_Token ret_token = getNextToken(&error);
        printf("DEBUG: Token: %d | Attr: %s | errorptr: %d\n", ret_token.type, ret_token.attr, error);
    }
    return 0;
}
