/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  TODO
 */

 #include "symtable.h"


/*
Inicializuje pole pro parametru funkce
@param *data - node v tabulce symbolu
*/
int arrParamInit(t_Data *data)
{
	data->arr_params = malloc(sizeof(t_ArrParams));
	if (data->arr_params == NULL)
		return MEMORY_ERROR;
	data->params_cnt = 0;


	data->arr_params->string = malloc(MAX_SIZE * sizeof(string));
	if (data->arr_params->string == NULL)
	{
		free(data->arr_params);
		return MEMORY_ERROR;
	}
	data->arr_params->size_alloc = MAX_SIZE;

	return MEMORY_OK;
}

/*
Prida jmeno parametru funkce do pole
@param *data - node v tabulce symbolu
@param s string se jmenem
*/
int arrParamAdd(t_Data *data, string s)
{
	if (data->params_cnt > data->arr_params->size_alloc)
	{
		data->arr_params->string = realloc(data->arr_params->string ,data->arr_params->size_alloc + MAX_SIZE);
		if (data->arr_params->string == NULL)
		{
			free(data->arr_params);
			return MEMORY_ERROR;
		}
	}

	if (stringInit(&(data->arr_params->string[data->params_cnt])) == MEMORY_ERROR)
		return MEMORY_ERROR;
	if (stringCopy(&(data->arr_params->string[data->params_cnt]), &s) == MEMORY_ERROR)
		return MEMORY_ERROR;

	data->params_cnt++;
	return MEMORY_OK;
}

/*
Uvolni pole parametru funkce
@param *data - node v tabulce symbolu
*/
void arrParamFree(t_Data *data)
{
	if (data->arr_params == NULL)
		return ;

	for (int i = 0; i < data->params_cnt; i++)
	{
		stringFree(&(data->arr_params->string[i]));
	}

	free(data->arr_params->string);
	free(data->arr_params);
	data->arr_params = NULL;
}

/* DEBUG function */
void arrPrintParam(t_Data *data)
{
	for (int i = 0; i < data->params_cnt; i++)
	{
		fprintf(stderr, "p%d: %s", i, data->arr_params->string[i].val);
	}
}
