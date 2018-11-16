#ifndef LISTE_H
#define LISTE_H


typedef enum {Text, Data, Bss} section_t;
typedef enum {Word, Byte, Asciiz, Space} directive_t; 

typedef enum {INIT_G,BSS,SPACE_BSS,DATA,SPACE_DATA,BYTE,WORD,ASCIIZ,TEXT,SYMB,OFFSET}ETAT_G;



typedef struct maillon{
	void* pval;
	struct maillon* suiv;
	}* Liste;

typedef struct operande{
	char * nom;
	char * type;
	}*operande_t;

typedef struct instruction{
	section_t section; 
	int ligne;
	unsigned int decalage;
	int nb_op_instr;
	operande_t op;
	}* instruction_t;

typedef struct donnee{
	directive_t directive;
	int nligne;
	int decalage;
	char* nom ;
	union{
		char byte;
		int word;
		char* asciiz;
		char* etiquette;
		unsigned int space;
		}valeur;
	}*donnee_t;

typedef struct etiquette{
	char * nom;
	int nligne;
	section_t section;
	int decalage;
	}*etiquette_t;



Liste creer_liste();

int est_vide(Liste L);

void visualiser_liste(Liste L, int cas);

Liste ajout_tete_liste(void* val, Liste L);

Liste ajout_queue( Liste L,void* val);

void *retirer_tete (Liste*L);

#endif		
