#include <stdio.h>
#include <stdlib.h>
#include "file.h"

File *initialiser()
{
    File *file = malloc(sizeof(*file));
    file->premier = NULL;

    return file;
}


void enfiler(File *file, lex_t lex)
{
    Element *nouveau = malloc(sizeof(*nouveau));
    if (file == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouveau->lexeme = lex;
    nouveau->suivant = NULL;

    if (file->premier != NULL) /* La file n'est pas vide */
    {
        /* On se positionne à la fin de la file */
        Element *elementActuel = file->premier;
        while (elementActuel->suivant != NULL)
        {
            elementActuel = elementActuel->suivant;
        }
        elementActuel->suivant = nouveau;
    }
    else /* La file est vide, notre élément est le premier */
    {
        file->premier = nouveau;
    }
}


lex_t defiler(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    lex_t lexdef;
    lexdef.val_lex = NULL;
    lexdef.ligne = 0;
    lexdef.nature = NULL;

    /* On vérifie s'il y a quelque chose à défiler */
    if (file->premier != NULL)
    {
        Element *elementDefile = file->premier;

        lexdef = elementDefile->lexeme;
        file->premier = elementDefile->suivant;
        free(elementDefile);
    }

    return lexdef;
}

void afficherFile(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *element = file->premier;
    lex_t lexLu;
    lexLu.val_lex = NULL;
    lexLu.ligne = 0;
    lexLu.nature = NULL;
    

    while (element != NULL)
    {
        lexLu = element->lexeme;
        printf ("nom : %s\n", lexLu.val_lex);	
       	printf ("ligne : %d\n", lexLu.ligne);
	printf ("type de lexeme : %s\n ", lexLu.nature);
        printf("\n");	
        element = element->suivant;
    }

    printf("\n");
}



