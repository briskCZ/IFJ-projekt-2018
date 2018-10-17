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
Prevede string na ciselnou hodnotu
@param str - vstupni string
@return unsigned long ciselna hodnota, ktera reprezentuje vstupni string
*/
unsigned hash(string *s);

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
void tableDestroy(t_symTable *t)
{
	if (t->root != NULL)
	{
		t_symTable left, right;
		left.root = t->root->left;
		right.root = t->root->right;
		
		tableDestroy(&left);
		tableDestroy(&right);
		deleteNode(t->root);
	}
}

/* hleda ve stromu idenditu s tokenem
 - pokud nalezne vrati ukazatel na danou polozku v tabulce, pokud ne tak NULL  */
t_Node* tableSearchToken(t_symTable *table, t_Token token)
{
	int key = hash(&token.attr);	
	t_Node *temp = table->root;	

	while (temp != NULL)
	{
		if (temp->data->key >= key)
		{
			if (stringCompare(temp->data->s, &token.attr)) //muze nastat kolize
				return temp; //pokud se rovna i ve stringu tak jsem ho nalezli
			else
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
void tableInsertToken(t_symTable *table, t_Token token)
{
	t_Node *find;
	if ((find = tableSearchToken(table, token)) == NULL)
	{
		t_Node *new;
		if ((new = createNode(token)) == NULL)
			return ; // TODO

		if (table->root == NULL) //first
			table->root = new;
		else
		{
			t_Node *temp = table->root;
			while (temp!= NULL)
			{
				if (temp->data->key < new->data->key)
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
		}
	}
	else
	{
		//prepsani hodnot
		;//stringInsert(find->data->s, token.attr.val); 
	}
}



/* ---- lokalnich funkce ---- */

unsigned hash(string *str)
{
	unsigned key = 0;
	int len = stringGetLength(str);
	char *chr = stringGet(str);
		
	for(int i = 0; i < len; i++)
	{
		key += (i+1) * (int)chr[i];
	}
		
	return key;
}

//Vytvori uzel z dat tokenu
t_Node* createNode(t_Token token)
{
	t_Node *new = malloc(sizeof(t_Node));
	if (new == NULL)
		return  NULL;
	
	new->left = new->right = NULL;
	
	new->data = malloc(sizeof(t_Data));
	if (new->data == NULL)
	{
		free(new);
		return NULL;
	}

	new->data->s = malloc(sizeof(string));
	if (new->data->s == NULL)
	{
		free(new->data);
		free(new);
		return NULL;
	}

	new->data->key = hash(&token.attr);
	stringInit(new->data->s);
	stringInsert(new->data->s, token.attr.val); //TEST 

	return new;
}

//Uvolni uzel (zrusi)
void deleteNode(t_Node *n)
{
	stringFree(n->data->s);
	free(n->data->s);
	free(n->data);
	free(n);
}
