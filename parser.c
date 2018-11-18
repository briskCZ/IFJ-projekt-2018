#include "scanner.h"
#include "ret_vals.h"

#define OK 0
#define ERROR 1
int error;

void sec1();
void sec2();
int term(t_Token token);
int param1();

int param2(t_Token token);
int param22();


int expr(t_Token token){
	//tady se bude volat vyhodnocování výrazů. vrací OK nebo ERROR;
	printf("-expr\n");
	printToken(token, error); //debug
	//printToken(token, error); //debug
	if(token.type == NIL){
		return OK;
	}else{
		printf("EXPR error expr expected on line: %d\n", sc_line_cnt);
		//return(ERROR);
		exit(1);
	}
}

void f_call(t_Token token){
	printf("-f_call\n");
	if (token.type == LEFT_PAR){
		param1();
		printf("F_CALL() succesfull on line: %d\n", sc_line_cnt);
		return;
	}else{
		if(param2(token) == T_EOL){
			printf("F_CALL bez () succesfull on line: %d\n", sc_line_cnt);
			return;
		}
	}
}

void assign(){
	printf("-assign\n");
	t_Token token = getNextToken(&error);
	if(expr(token)==OK){
		printf("ASSIGN succesfull on line: %d\n", sc_line_cnt);
		return;
	}else{
		f_call(token);
		return;
	}
}

void id_eval(){
	printf("-id_eval\n");
	t_Token token = getNextToken(&error);
	if(token.type == ASSIGNMENT){
		assign();
		return;
	}else{
		f_call(token);
		return;
	}
}

void code(t_Token token){
    printf("-code\n");
	//printToken(token, error); //debug
	if(token.type == ID){
		printf("-prirazeni \n");
		id_eval();
		return;
	}
	else if(token.type == IF){
		printf("-if \n");
		if(expr(getNextToken(&error))==OK){
			token = getNextToken(&error);
			if (token.type == THEN){
				printf("-if THEN\n");
				token = getNextToken(&error);
				printToken(token, error); //debug
				if(token.type == T_EOL){
					sec2();
					token = getNextToken(&error);
					if (token.type == T_EOL){
						printToken(token, error); //debug
						sec1();
						printf("IF succesfull on line: %d\n", sc_line_cnt);
						return;
					}
					else if (token.type == ELSE){
						printf("-if ELSE\n");
						return;
					}
					
				}else{
					printToken(token, error); //debug
					printf("IF error1 EOL expected on line: %d\n", sc_line_cnt);
					return;
				}
			}else{
				printf("IF error THEN expected on line: %d\n", sc_line_cnt);
				return;
			}
		}else{
			printf("IF error expr expected on line: %d\n", sc_line_cnt);
			return;
		}
	}
	else if(token.type == WHILE){
		printf("-while\n");
		if(expr(getNextToken(&error))==OK){
			token = getNextToken(&error);
			if(token.type == DO){
				printf("-while DO\n");
				token = getNextToken(&error);
				if(token.type == T_EOL){
					sec1();
					printf("WHILE succesfull on line: %d\n", sc_line_cnt);
					return;
				}else{
					printf("WHILE error EOL expected on line: %d\n", sc_line_cnt);
					return;
				}
			}else{
				printf("WHILE error DO expected on line: %d\n", sc_line_cnt);
				return;
			}
		}else{
			printf("WHILE error expr expected on line: %d\n", sc_line_cnt);
			return;
		}
		
		return;
	}
	else{
		if (expr(token)==OK){
			return;
		}else{
			printf("EXPR error expr on line: %d\n", sc_line_cnt);
		}
		
	}
}

void sec1(){
	printf("-sec1\n");
    t_Token token = getNextToken(&error);
	printToken(token, error); //debug
	if(token.type == END){
		printf("-sec1 END\n");
        return;
	}else{
		code(token);
		printf("-sec1 return z code\n");
		printToken(token, error); //debug
		return;
	}
}

void sec2(){
	printf("-sec2\n");
    t_Token token = getNextToken(&error);
	printToken(token, error); //debug
	if(token.type == ELSE){
		printf("-sec2 ELSE\n");
        return;
	}else{
		code(token);
		printf("-sec2 return z code\n");
		printToken(token, error); //debug
		return;
	}
}

int term(t_Token token){
    printf("-term\n");
    if (token.type == ID || token.type == DOUBLE ||
        token.type == INT || token.type == STRING){
        return OK;
    }
    return ERROR;
}

int param11(){
    printf("-param11\n");
    t_Token token = getNextToken(&error);
    //printToken(token, error); //debug
    if (token.type == RIGHT_PAR){
        //vrat dalsi, mozna eol
        token = getNextToken(&error);
        return token.type;
    }else if (token.type == COMMA){
        token = getNextToken(&error);
        if(term(token) == OK){
            printf("call: param11\n");
            param11();
        }else{
            printf("error wrong params line: %d\n", sc_line_cnt);
        }
    }
}

int param1(){
    printf("-param1\n");
    t_Token token = getNextToken(&error);
    //printToken(token, error);
    if (token.type == RIGHT_PAR){
        token = getNextToken(&error);
        //printToken(token, error);
        return token.type;
    }else if(term(token) == OK){
        if (param11() == T_EOL){
            return T_EOL;
        }else{
            printf("error eol expected line: %d\n", sc_line_cnt);
        }
    }else{
        printf("error right par or term expcted line: %d\n", sc_line_cnt);
    }
}

int param2(t_Token token){
    printf("-param2\n");
    if (token.type == T_EOL){
        return T_EOL;
    }else if(term(token) == OK){
		if(param22() == T_EOL){
			return T_EOL;
		}else{
			printf("PARAM2 error expected EOL on line: %d\n", sc_line_cnt);
		}
    }
}

int param22(){
    printf("-param22\n");
    t_Token token = getNextToken(&error);
    if (token.type == T_EOL){
        return T_EOL;
    }else if (token.type == COMMA){
        token = getNextToken(&error);
        if(term(token) == OK){
            param22();
        }else{
            printf("error wrong params22 line: %d\n", sc_line_cnt);
        }
    }
}



void fun_decl(){
    printf("-fun_decl\n");
    t_Token token = getNextToken(&error);
    //printToken(token, error);
    if (token.type == ID){
        token = getNextToken(&error);
        //printToken(token, error);
        if (token.type == LEFT_PAR){
            if (param1() == T_EOL){
                printf("-function decl correct line: %d\n", sc_line_cnt);
				sec1();
                return;
            }else{
                printf("error func decl line: %d\n", sc_line_cnt);
                return;
            }
        }else{
            printf("error lpar expected line: %d\n", sc_line_cnt);
            return;
        }
    }else{
        printf("error id expected line: %d\n", sc_line_cnt);
        return;
    }
}

void program(t_Token token){
	printf("-program\n");
	//printToken(token, error);
    switch (token.type){
        case T_EOF:
			printf("-program ended on line: %d\n",sc_line_cnt);
            return;
		case T_EOL:
            return;
        case DEF:
            fun_decl();
            break;
        default:
            code(token);
            break;
    }
}

int main(){
    t_Token token;
    //pridani znaku noveho radku, na zacatek souboru, kvuli blokovym komentarum
    scannerInit();

    do {
        token = getNextToken(&error);
        program(token);
        //printToken(token, error);
    } while (token.type != T_EOF);


    scannerClean();
    return 0;
}
