/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Implementace funkci pro VBS - vyhledavaci binarni storm
*/

#include "symtable.h"

//hlavicky lokalnich funkcich

/*
Vytvori uzel z dat tokenu
@param t - token
@return tNode inicializovany ukazatel
*/
t_Node* createNode(t_Token t);

/*
Uvolni uzel (zrusi)
@param n - uzel
@return void
*/
void deleteNode(t_Node *n);

/* ----------- */

//vytvori tabulku
t_symTable tableInit()
{
	t_symTable table;
	table.root = NULL;

	return table;
}

//odstrani tabulku
void tableDestroy()
{
	if (table.root != NULL)
	{
		t_symTable left, right;
		left.root = table.root->left;
		right.root = table.root->right;
		
		tableDestroy(&left);
		tableDestroy(&right);
		deleteNode(table.root);
	}
}

/* hleda ve stromu idenditu s tokenem
 - pokud nalezne vrati ukazatel na danou polozku v tabulce, pokud ne tak NULL  */
t_Node* tableSearchItem(string s)
{
	t_Node *temp = table.root;	
	while (temp != NULL)
	{
		//printf("%s | %s\n", s.val, table->root->data->name->val);
		if (stringCompare(temp->data->name, &s) == 0)
		{
			return temp;
		}
		else if ( stringCompare(temp->data->name, &s) < 0)
		{
			temp = temp->right;
		}
		else
		{
			temp = temp->left;
		}
	}
	return NULL;
}

/* vlozi token do tabulky
- pokud token neni nalezen tak se vytvori 
- v opacnem pripade se hodnoty prepisi(aktualizuji)
*/
int tableInsertToken(t_Token token)
{
	//printf("t: %s\n", token.attr.val);
	t_Node *find;
	if ((find = tableSearchItem(token.attr)) == NULL)
	{
		t_Node *new;
		if ((new = createNode(token)) == NULL)
			return MEMORY_ERROR;
		if (table.root == NULL) //first
			table.root = new;
		else
		{
			t_Node *temp = table.root;
			while (temp!= NULL)
			{
				if (stringCompare(temp->data->name, new->data->name) < 0)
				{
					if (temp->right == NULL)
					{
						temp->right = new;
						break;
					}
					temp = temp->right;
				}
				else
				{
					if (temp->left == NULL)
					{
						temp->left = new;
						break;
					}
					temp = temp->left;
				}
			}
			stringPrint(temp->data->name);
		}
	}

	return MEMORY_OK;
}

void tableAddParam(string *s)
{
	
	;
}

/*kdyz -1 tak neupravuji*/
void tableChangeItem(string *s, int is_var, int data_type, int defined, int global)
{
	t_Node *data = tableSearchItem(*s);
	if (data != NULL)
	{
		if (is_var != -1) data->data->is_var = is_var;
		if (data_type != -1) data->data->data_type = data_type;
		if (defined != -1) data->data->defined = defined;
		if (global != -1) data->data->global = global;
	}
	else
	{
		; //TODO
	}
}

/* ---- lokalnich funkce ---- */

//Vytvori uzel z dat tokenu
t_Node* createNode(t_Token token)
{
	t_Node *new = malloc(sizeof(t_Node));
	if (new == NULL)
		return NULL;
	
	new->left = new->right = NULL;
	
	new->data = malloc(sizeof(t_Data));
	if (new->data == NULL)
	{
		free(new);
		return NULL;
	}
	new->data->local_symTable = malloc(sizeof(t_symTable));
	if (new->data->local_symTable == NULL)
	{
		free(new->data);
		free(new);
		return NULL;
	}
	new->data->local_symTable->root = NULL;

	//new->data->list = malloc(sizeof(string *));
	
	new->data->name = malloc(sizeof(string));
	if (new->data->name == NULL)
	{
		free(new->data);
		free(new);
		return NULL;
	}

	stringInit(new->data->name);
	stringInsert(new->data->name, token.attr.val);
	return new;
}

//Uvolni uzel (zrusi)
void deleteNode(t_Node *n)
{
	stringFree(n->data->name);
	tableDestroy(n->data->local_symTable);
	//free(n->data->list);
	free(n->data->name);
	free(n->data);
	free(n);
}
