/*
 * jednotkovy test pro zdrovy soubor - symtable.c
*/

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "scanner.h"
#include "symtable.h"

#define p(x) printf("%s\n", x);

void tablePrint(t_Node *root)
{
    if (root != NULL)
    {
        tablePrint(root->left);
        printf("%u\n", root->data->key);
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
	p("init table...");
    t_symTable table = tableInit();

	p("\ninsert element 'ahoj'");
    sInsert(&table, 2, "ahoj");

	p("\ninsert element 'ahojs'");

    sInsert(&table, 2, "ahojs");
	p("\ninsert element 'ahjs'");
    sInsert(&table, 2, "ahjs");


    tablePrint(table.root);
	
	tableDestroy(&table);
    return 0;
}

