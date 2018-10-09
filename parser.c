#include "scanner.h"
#include "ret_vals.h"

int main(){
    while(1){
        t_Token ret_token = getNextToken();
        printf("DEBUG: Token: %d | Attr: %s\n", ret_token.type, ret_token.attr);
    }
    return 0;
}
