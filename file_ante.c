#include "file_ante.h"
#include <stdlib.h>
#include <stdio.h>


File creer_file(void){
	return NULL;
}

int file_vide(File f){
	return !f;
}

File enfiler(Lex_t lex, File f){
	File p = calloc(1,sizeof(*p));
	if (p==NULL) return NULL;
	if (file_vide(f)) {
		p->lexeme=lex; p->suiv = p; }
	else {
		p->lexeme=lex; p->suiv=f->suiv ;
		f->suiv=p;
	}
	return p;
}

Lex_t defiler(File *pf){
	File p = (*pf)->suiv;
	if (p->suiv==p){
		Lex_t lex = p->lexeme;
		free(p);   
		*pf=NULL;
		return lex;
	}
	Lex_t lex = p->lexeme;
	(*pf)->suiv = p->suiv;
	free(p);
	return lex;
}

void visualiser_file(File f){
	if (!file_vide(f)){
		Lex_t lex;
		File p = f;
		printf ("analyse lexicale\n");
		/* while(p!=NULL){ */
		/*   lex = p->lexeme; */
		/*   printf ("nom : %s\n", lex.val_lex); */
		/*   printf ("ligne : %d\n", lex.ligne);  */
		/*   printf ("type de lexeme : %s\n " , lex.nature); */
		/*   p=p->suiv; */
		/*   printf("\n"); */
		/* } */
		/* printf("\n"); */
		while (p!=f){
			lex=(*p).lexeme;
                        printf ("nom : %s\n", (p->lexeme).val_lex);
			printf ("ligne : %d\n", ((*p).lexeme).ligne);
			printf ("type de lexeme : %s\n " , ((*p).lexeme).nature);
			printf("\n");
			p=p->suiv;
			}
		
	        printf ("nom : %s \n", (p->lexeme).val_lex);
		printf ("ligne : %d\n", ((*p).lexeme).ligne);
		printf ("type de lexeme : %s\n " , ((*p).lexeme).nature);
 
		printf("\n ");
	}
	else printf("f est vide.\n");
}
