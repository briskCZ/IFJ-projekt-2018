/*  IFJ Projekt 2018 "Implementace prekladace imperativniho jazyka IFJ18"
 *  Autori:
 *      Zdenek Dolezal (xdolez82)
 *      Marek Nesvadba (xnesva06)
 *      David Miksanik (xmiksa05)
 *  Fakulta informacnich technologii VUT v Brne
 *  Popis souboru:
 *  Navratove hodnoty prekladace
*/
#ifdef debug
    #define P(a) fprintf(stderr, "%s\n", a);
#else
    #define P(a) ;
#endif

#define SUCCESS 0
#define ERROR_LEX 1
#define ERROR_SYNTAX 2
#define ERROR_SEMANTIC 3
#define ERROR_SEM_COMPATIBILITY 4
#define ERROR_SEM_PARAM 5
#define ERROR_SEM_OTHER 6
#define ERROR_ZERO_DIV 9
#define ERROR_INTERNAL 99

/* Navratove hodnoty behu internich casti */
/* Pro kazdou, ktera alokuje dynamicky */
#define MEMORY_OK 0
#define MEMORY_ERROR 1
#define STACK_ERROR -10
