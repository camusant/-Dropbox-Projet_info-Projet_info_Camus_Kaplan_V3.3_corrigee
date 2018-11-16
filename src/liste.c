#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <global.h>
#include <notify.h>
#include "file.h"
#include "lex_file.h"
#include "liste.h"
#include "gram.h"

const char * type_de_section[] ={".text",".data",".bss"};
const char * type_de_directive[] ={".word",".byte",".asciiz",".space"};


Liste creer_liste(){
	return NULL;
}

int est_vide(Liste L){
	if(L==NULL){return 0;}
	return 1;
}

void visualiser_liste(Liste L,int cas){
	Liste p = L;
	donnee_t donnee;
	instruction_t instruction;
	switch (cas){
            	case 1:  /*cas où on est sur une directive de type .data */
			printf("section .data");
			while (p!=NULL){
				donnee=p->pval;
				printf ("la directive est %s\n",type_de_directive[(*donnee).directive]);
				printf("la ligne est est %d\n" ,(*donnee).nligne);
				printf("le decalage est %d\n" ,(*donnee).decalage);
				/* printf("l'operande est de type %s\n",type_de_lexeme[(*donnee).nom]); */
				p=p->suiv;
				}
			break;
	        case 2: /*Directive de type .bss*/
			printf("section .bss");
			while (p!=NULL){
				donnee=p->pval;
				printf ("la directive est %s\n",type_de_directive[(*donnee).directive]);
				printf("la ligne est est %d\n" ,(*donnee).nligne);
				printf("le decalage est %d\n" ,(*donnee).decalage);	
				p=p->suiv;
				}
			break;
		case 3:
			printf("section .text");
 			while (p!=NULL){
				instruction=p->pval;
				printf ("la section est %s\n",type_de_section[(*instruction).section]);
				printf("la ligne est est %d\n" ,(*instruction).ligne);
				printf("le decalage est %d\n" ,(*instruction).decalage);
				printf("le nombre d'opérande est %d\n" ,(*instruction).nb_op_instr);
				p=p->suiv;
				}
			break;
	       default:
		        break;
	}
}

Liste ajout_tete_liste(void* val, Liste L){
	Liste p=calloc(1,sizeof(*p));
 	if (p==NULL) return NULL;
 	p->pval=val;
	p->suiv=L;
 	return p;
}

Liste ajout_queue( Liste L,void* val){
	if(L==NULL){
		L=ajout_tete_liste(val,L);
		return L;
	}
	Liste p=calloc(1,sizeof(*p));
 	if (p==NULL) return NULL;

	p->pval = val;
	p->suiv = NULL;
	Liste m = L;
	for(m=L; m->suiv != NULL; m=m->suiv){
	}
	m->suiv = p;
	return L;
}

void* retirer_tete(Liste *L){
	if(*L==NULL){return NULL;}
	Liste tete =(*L);
	if(tete->suiv == NULL){
		void* p = tete->pval;
		free(tete);
		L = NULL;
		return p;
	}
	else{
		void* c = tete->pval;
		(*L)= tete->suiv;
		free(tete);
 		return c;
	}
}
