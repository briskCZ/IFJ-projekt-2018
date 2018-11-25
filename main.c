#include <stdio.h>
#include <stdlib.h>

char* ramec(){
//	char* a = (is_in_func == 0) ? "GF" : "LF";
	char *a = "GF";
	return a;
}

int temp_label = 1;
int temp_num = 0;

void convertTypesAdd()
{
	int aux = temp_label;

	printf("DEFVAR %s@$%d\n", ramec(), temp_num++);
	printf("DEFVAR %s@$%d\n", ramec(), temp_num);

	//typ un1 je ulozen v temp_num-1
	printf("DEFVAR %s@$t%d\n", ramec(), 0);
	printf("POPS %s@$t%d\n", ramec(), 0);
	//typ un2 je ulozen v temp_num
	printf("DEFVAR %s@$t%d\n", ramec(), 1);
	printf("POPS %s@$t%d\n", ramec(), 1);

	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num-1, ramec(), 0);
	printf("TYPE %s@$%d %s@$t%d\n", ramec(), temp_num, ramec(), 1);

	printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label++, ramec(), temp_num-1);
		printf("JUMPIFNEQ $L%d string@string %s@$%d\n", temp_label, ramec(), temp_num);
		//	genAddStr();
			printf("JUMP $LEND%d\n", aux);
		printf("LABEL $L%d\n", temp_label--);
			printf("EXIT int@1\n"); //ERROR oba nejsou string

	printf("LABEL $L%d\n", temp_label);
	temp_label += 2;
		printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label++, ramec(), temp_num-1);
			printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), temp_num);
				printf("PUSHS %s@$t%d\n", ramec(), temp_num-1);
				//genAddS();
				printf("JUMP $LEND%d\n", aux);
			printf("LABEL $L%d\n", temp_label++);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), temp_num);
					printf("INT2FLOATS\n");
					printf("PUSHS %s@$t%d\n", ramec(), temp_num-1);
					//genAddS();
					printf("JUMP $LEND%d\n", aux);
				printf("LABEL $L%d\n", temp_label);
					printf("EXIT int@1\n"); //ERROR oba nejsou int nebo float

		temp_label -= 2;
		printf("LABEL $L%d\n", temp_label);
		temp_label += 3;
			printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label++, ramec(), temp_num-1);
				printf("JUMPIFNEQ $L%d string@float %s@$%d\n", temp_label, ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), temp_num);
					printf("PUSHS %s@$t%d\n", ramec(), temp_num-1);
					//genAddS();
					printf("JUMP $LEND%d\n", aux);
				printf("LABEL $L%d\n", temp_label++);
					printf("JUMPIFNEQ $L%d string@int %s@$%d\n", temp_label, ramec(), temp_num);
						printf("PUSHS %s@$t%d\n", ramec(), temp_num);
						printf("PUSHS %s@$t%d\n", ramec(), temp_num-1);
						printf("INT2FLOATS\n");
						//genAddS();
						printf("JUMP $LEND%d\n", aux);
					printf("LABEL $L%d\n", temp_label);
					temp_label -= 2;
						printf("EXIT int@1\n"); //ERROR oba nejsou int nebo float
			printf("LABEL $L%d\n", temp_label);
			temp_label += 3;
				printf("EXIT int@1\n"); //ERROR oba nejsou int nebo float

	printf("LABEL $LEND%d\n", aux);

}


int main()
{
	printf(".IFJcode18\n");
	printf("PUSHS string@%s\n", "asdf");
	printf("PUSHS int@2\n");

	convertTypesAdd();
	printf("BREAK\n");


	return 0;
}
