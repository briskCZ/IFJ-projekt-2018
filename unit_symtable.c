/*
 * jednotkovy test pro zdrovy soubor - symtable.c
*/

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "scanner.h"
#include "symtable.h"

#define p(x, y) printf("%s '%s'\n", x, y);

int a;
void tablePrint(t_Node *root)
{
    if (root != NULL)
    {
        printf("[%d] %s\n", a++, root->data->name->val);
        tablePrint(root->left);
        tablePrint(root->right);
    }
}

void sInsert(t_symTable *table, int itype, char *is)
{
	string s;
	stringInit(&s);
	stringInsert(&s, is);

	t_Token t = { .type = itype, .attr = s};	
	tableInsertToken(table, t);

	stringFree(&s);
}

int main()
{
	printf("=== INIT TABLE ===\n");
    t_symTable table = tableInit();

	char str[][3] = {"b", "e", "a", "d", "c"};
	for (int i = 0; i < 5; i++)
	{
		p("insert element: ", str[i]);
		sInsert(&table, 2, str[i]);
	}

	printf("\n=== PRINT TABLE ===\n");
	a = 0;
    tablePrint(table.root);
	
	tableDestroy(&table);


    return 0;
}

