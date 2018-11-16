#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <ctype.h>
#include <global.h>
#include <notify.h>
#include "lex_file.h"
#include "liste.h"
#include "file.h"
#include "gram.h"


char *strdup(const char *string){
  char *d= malloc (strlen (string) + 1);
  if(d==NULL) return NULL;  /*No memeory*/
  strcpy(d,string);
  return d;
  
}

dico * lect_dico_int(char * nomFichierDico, int * pointNombreInstruc){
  /* Le but de cette fonction est de charger le dictionnaire en mémoire*/
  /* A partir du fichier pointé par nomFichierDico  */
  FILE * f1 = NULL;
  int i;
  char s1[512];
  dico * tab;
  /*lecture du fichier*/
  f1 =fopen( nomFichierDico, "r");
  /*Contrôle de présence*/
  if(f1==NULL){
    printf("Fichier non present\n");
    return NULL;
  }
   
  /*Contrôle de la présence d'un entier sur la première ligne*/
  if(fscanf(f1,"%d", pointNombreInstruc) !=1){
    printf("Erreur: pas de nombre d instructions\n");
    return NULL;
  }
   
  tab = calloc( *pointNombreInstruc, sizeof(*tab));
  if(tab==NULL){
    printf("Erreur: pointeur de dico non charge\n");
    return NULL;
  }
 
  for (i=0; i<*pointNombreInstruc; i++){
    if( fscanf(f1, "%s %c %d", s1, &(tab[i].type), &(tab[i].nb_op)) !=3){
      free(tab);
      return NULL;
    }
    tab[i].symbole = strdup( s1 );
  }
  fclose(f1);
  printf("Dictionnaire bien lu\n");
  return tab;
}



const char * type_section(section_t t) {
	char *sectionTab[] = {".text",".data",".bss"};
	return sectionTab[t];
}

const char * type_directive(directive_t t) {
	char *dirTab[] = {".word",".byte",".asciiz",".space"};
	return dirTab[t];
}



/**
 * fonction automate
 * @param liste_data, liste_bss, liste_text : nouvelles collections de lexeme
 * @param file : file de lexeme de l'analyse lexicale
 * @param dico : fichier texte chargé en mémoire: dictionnaire d'instruction
 * @return 1 if true ou fin de la chaine analysee, 0 en cas d'erreur ou de mauvais caractère.
 * @brief performe l'analyse grammaticale
 */
int automate(Liste* liste_text, Liste* liste_data, Liste* liste_bss, File* file, dico* dico, int taille_dico){
        Element * p = file->premier;
	etiquette_t etiquette;
	int ligne;
	lex_t lexeme;
	char* nom;
	char * type_de_lexeme;
	int nb_op,ctp_op;
	int ctp_bss,ctp_data,ctp_instr;
	
	ETAT_G ETAT = INIT_G;
	donnee_t donnee;
	instruction_t instruction;
	while (p->suivant != file->premier){
	        lexeme=(*p).lexeme;
		printf ("%s",lexeme.val_lex);
		
		switch(ETAT) {
			case INIT_G :
				if (strcmp(lexeme.val_lex, type_section(Bss) )==0){ETAT=BSS;}
				if (strcmp(lexeme.val_lex, type_section(Data) )==0){ETAT=DATA;}		
				if (strcmp(lexeme.val_lex, type_section(Text) )==0){ETAT=TEXT;}
				break;
			
			case BSS :
				if (strcmp(lexeme.val_lex, type_section(Data) )==0){ETAT=DATA;}		
				if (strcmp(lexeme.val_lex, type_section(Text) )==0){ETAT=TEXT;}
				if (strcmp(lexeme.val_lex,type_directive(Space))==0){ETAT=SPACE_BSS;}
				break;
			case SPACE_BSS :
				if (lexeme.val_lex=='\n'){ETAT=BSS;}
				else if (lexeme.nature=="[DECIMAL]"){
					(*donnee).directive=Space;
					(*donnee).nligne=lexeme.ligne;
					(*donnee).decalage=ctp_bss;
					(*donnee).nom=lexeme.nature;
					*liste_bss=ajout_queue(*liste_bss,donnee);
					}
				else if (lexeme.nature=="[HEXA]"){
					(*donnee).directive=Space;
					(*donnee).nligne=lexeme.ligne;
					(*donnee).decalage=ctp_bss;
					(*donnee).nom=lexeme.nature;
					*liste_bss=ajout_queue(*liste_bss,donnee);
					}
				else if (lexeme.nature=="[ETIQUETTE]"){
					
					}
				break;
			
			
			case DATA :
				if (strcmp(lexeme.val_lex, type_section(Bss) )==0){ETAT=BSS;}		
				if (strcmp(lexeme.val_lex, type_section(Text) )==0){ETAT=TEXT;}
				if (strcmp(lexeme.val_lex,type_directive(Word))==0){ETAT=WORD;}
				if (strcmp(lexeme.val_lex,type_directive(Byte))==0){ETAT=BYTE;}
				if (strcmp(lexeme.val_lex,type_directive(Space))==0){ETAT=SPACE_DATA;}
				if (strcmp(lexeme.val_lex,type_directive(Asciiz))==0){ETAT=ASCIIZ;}
				break;
			case SPACE_DATA :
				if (lexeme.val_lex=='\n'){ETAT=DATA;}
				else if (lexeme.nature=="[DECIMAL]"){
					(*donnee).directive=Space;
					(*donnee).nligne=lexeme.ligne;
					(*donnee).decalage=ctp_data;
					(*donnee).nom=lexeme.nature;
					*liste_data=ajout_queue(*liste_data,donnee);
					}
				else if (lexeme.nature=="[HEXA]"){
					(*donnee).directive=Space;
					(*donnee).nligne=lexeme.ligne;
					(*donnee).decalage=ctp_data;
					(*donnee).nom=lexeme.nature;
					*liste_data=ajout_queue(*liste_data,donnee);
					}
				break;
			case BYTE :
				if (lexeme.val_lex=='\n'){ETAT=DATA;}
				else if (lexeme.nature=="[DECIMAL]"){
					(*donnee).directive=Byte;
					(*donnee).nligne=lexeme.ligne;
					(*donnee).decalage=ctp_data;
					(*donnee).nom=lexeme.nature;
					*liste_data=ajout_queue(*liste_data,donnee);
					ctp_data+=1;
					}
				else if (lexeme.nature=="[HEXA]"){
					(*donnee).directive=Byte;
					(*donnee).nligne=lexeme.ligne;
					(*donnee).decalage=ctp_data;
					(*donnee).nom=lexeme.nature;	
					*liste_data=ajout_queue(*liste_data,donnee);
					ctp_data+=1;
					}
				break;
			case WORD :
				if (lexeme.val_lex=='\n'){ETAT=DATA;}
				else if (lexeme.nature=="[DECIMAL]"){
					(*donnee).directive=Word;
					(*donnee).nligne=lexeme.ligne;
					(*donnee).decalage=ctp_data;
					(*donnee).nom=lexeme.nature;
					*liste_data=ajout_queue(*liste_data,donnee);
					ctp_data+=4;
					}
				else if (lexeme.nature=="[HEXA]"){
					(*donnee).directive=Word;
					(*donnee).nligne=lexeme.ligne;
					(*donnee).decalage=ctp_data;
					(*donnee).nom=lexeme.nature;	
					*liste_data=ajout_queue(*liste_data,donnee);
					ctp_data+=4;
					}
				break;
			case ASCIIZ :
				if (lexeme.val_lex=='\n'){ETAT=DATA;}
				break;


			/*case TEXT :
				if
				break;	*/	        

			default:
			        break;
		}
	}

	return 1;
}
