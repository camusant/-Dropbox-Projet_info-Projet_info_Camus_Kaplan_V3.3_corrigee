#ifndef GRAM_H
#define GRAM_H

#include "file.h"
#include "lex_file.h"
#include "liste.h"

typedef struct symb {
	char* symbole;
	char type;
	int nb_op;
	} dico;


char *strdup(const char *string);

dico * lect_dico_int(char * nomFichierDico, int * pointNombreInstruc);

int automate(Liste* liste_text, Liste* liste_data, Liste* liste_bss, File * file, dico* dico, int taille_dico);

#endif
