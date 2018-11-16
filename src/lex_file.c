#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "global.h"
#include "notify.h"
#include "lex_file.h"
#include "file.h"
#include "liste.h"
#include "gram.h"

/**
 * @param file Assembly source code file name.
 * @param nlines Pointer to the number of lines in the file.
 * @return should return the collection of lexemes
 * @brief This function loads an assembly code from a file into memory.
 *
 */
void lex_load_file( char *file, unsigned int *nlines ) {

    FILE        *fp   = NULL;
    File        *filetotale;  /*filetotale est la file de lexèmes totale*/ 
    filetotale = initialiser();
    char        line[STRLEN]; /* original source line */
    dico *  dicoDefInstruction ;
    int   nbDefInstructions;
    int i;


    fp = fopen( file, "r" );
    if ( NULL == fp ) {
        /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
        ERROR_MSG("Error while trying to open %s file --- Aborts",file);
    }

    *nlines = 0;

    while(!feof(fp)) {

        /*read source code line-by-line */
        if ( NULL != fgets( line, STRLEN-1, fp ) ) {
            line[strlen(line)-1] = '\0';  /* eat final '\n' */
            (*nlines)++;

            if ( 0 != strlen(line) ) {
	      lex_read_line(line,*nlines, filetotale);
            }
        }
    }
    
    printf("Affichage totale :\n");
    afficherFile( filetotale);

    dicoDefInstruction =  lect_dico_int("dico_instructions.txt", &nbDefInstructions);

    printf("nombres d'instructions : %d\n", nbDefInstructions);
    /* /\*Affichage du dictionnaire*\/ */
    /* for (i=0; i<nbDefInstructions; i++){ */
    /*   printf("symbole %d : ", i+1); */
    /*   puts( dicoDefInstruction[i].symbole); */
    /*   printf("type %d : %c \n", i+1, dicoDefInstruction[i].type); */
    /*   printf("Nombres d'operandes : %d \n", dicoDefInstruction[i].nb_op); */
    /* } */

    

    free(filetotale);

    fclose(fp);
    return;
}

/**

 * @param line String of the line of source code to be analysed.
 * @param nline the line number in the source code.
 * @return should return the collection of lexemes that represent the input line of source code.
 * @brief This function performs lexical analysis of one standardized line.
 *

 */

void lex_read_line( char *line, int nline, File * ftotal) {
    char* token = NULL;
    char* current_address=line;
    File* fileligne;
    lex_t lexemeLu;

    fileligne = initialiser();

    /* TODO : faire l'analyse lexical de chaque token ici et les ajouter dans une collection*/
    while( (current_address= newGetNextToken(&token, current_address, nline, fileligne)) != NULL){ 

        /* puts(token); */
    }

    
    
    /* printf("Affichage ligne\n"); */
    /* afficherFile(fileligne); */
    while(fileligne->premier != NULL){
        lexemeLu = defiler(fileligne);
        enfiler(ftotal,lexemeLu);
    }
    free(fileligne);
    /* printf("Affichage total\n"); */
    /* afficherFile(ftotal); */
    return;
}


char* newGetNextToken(char** token, char* current_line, int nline2, File * fligne)
{
  /*fligne est un pointeur de la  file de la ligne*/
    
    char* start = current_line; 
    char* end=NULL;
    char * commentaire = current_line;
    int token_size=0;  
    int retourEtat;
    lex_t remplissage;
    File * filetoken; /*filetoken est la File du Token*/

    filetoken = initialiser();
    /*Contrôles des commentaires*/

    if (start[0]=='#')
    {
        remplissage.nature = "[COMMENT]";
        remplissage.val_lex = commentaire;
	remplissage.ligne = nline2;
        enfiler(fligne , remplissage);	
        /* printf("[COMMENT]"); */
        /* puts(current_line); */
	free(filetoken);
	return NULL;
	
    }
      
    /* check input parameter*/ 
    if (current_line ==NULL){
      free(filetoken);
      return NULL;
      
    }
    
    
    /* eats any blanks before the token*/
    while (*start!='\0'  && ( isblank(*start) || *start==',' )){
	start++;
    }	
    /* go till next blank or end*/ 
    end=start; 

    while (*end!='\0'  && !isblank(*end) && *end!=',' ){
	end++; 
    }
    /*compute size : if zero there is no more token to extract*/
    printf("start:");
    puts( start);
    printf("end:");
    puts( end);
    
    token_size=end-start;    /*A la fin d'une ligne, *start=='/0' et token_size==0 */
   
    if (token_size>0){
	*token 	= calloc(token_size+1,sizeof(*start));
	strncpy(*token,start,token_size);
	(*token)[token_size]='\0';
	retourEtat = machine_etat(*token, nline2, filetoken );
	/* if( retourEtat==1 ) printf("Lecture reussi\n"); */
	/* printf("Affichage filetoken\n"); */
	/* afficherFile(filetoken); */
	enfiler(fligne, (filetoken->premier)->lexeme);
	return end;
    }
    
    if (token_size==0){
      /*Modification3*/
      remplissage.nature = "[RETOUR_LIGNE]";
      remplissage.val_lex = "Retour_ligne";
      remplissage.ligne = nline2;
      enfiler(fligne, remplissage );
      /*Fin modification3*/
    }
    
    free(filetoken);
    return NULL;
}  

/**
 * fonction machine_etat
 * @param text, de type char *, la chaine de caractère, lu caractère par caractère pour déterminer son type
 * @param etat, entier qui indique l'etat de la machine dans la fonction.
 * @param nredline, entier numéro de la ligne lu
 * @return 1 if true ou fin de la chaine analysee, 0 en cas d'erreur ou de mauvais caractère.
 * @brief Cette fonction permat de lire une chaine de caractères, caractère par caractère, pour déterminer son type
 * @results
 */

int  machine_etat( char * text, int nredline, File * flu ){
  int l=nredline;
  lex_t e ;
  int c='a';
  int i=0;
  int etat=INIT;
  char* p = calloc(1,sizeof(*p));
  while(c!='\0'){/*Pas encore arrive, au bout de texte*/
    /* printf("etat= %d\n",etat); */
    c = text[i]; /*Caractere lu*/
    *p=(char)c;
    
    switch( etat)
    {
    case INIT: /*Demerrage*/
      if ( isdigit ( c )) {  /* isdigit : c est un digit*/ 
	if (c=='0'){
	  etat = DECIMAL_ZERO;/*c different de 0*/
        }
        else etat=DECIMAL;
      }
      else if( islower(c) ) etat=LETTRE_MIN; /*c est une lettre minuscule*/
      else if( isupper(c) ) etat=LETTRE_MAJ; /*c est une lettre majuscule*/
      else if( c=='_' || c=='-') etat=SYMBOLE;
      else if( c=='$' ) etat=REGISTRE;
      else if( c!='.' && ispunct(c) ){
	e.nature="[PONCTUATION]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu, e);
	/* printf("[PONCTUATION \t]\t"); */
	/* puts(text); */
	/* printf("\n"); */
	free(p);
	return 1;
      }
      else if( c=='.') etat=POINT; 
      else if(c=='\0'){
        e.nature="[POINT]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu,e);
	/* printf("fin du texte des le debut\n"); */
        /* puts(text); */
	/* printf("\n"); */
	free(p);
	return 1; /*c est la fin de text*/
      }
      else{  /*erreur de lecture*/
	e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break;
	 
    case DECIMAL_ZERO:
      if (c=='x' || c=='X') etat= DEBUT_HEXA; /*debut d un hexa*/
      else if( isdigit(c) ) etat= DECIMAL;
      else if(c=='\0'){
	e.nature="[DECIMAL]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu,e);	
	/*   printf("[DECIMAL \t]\t"); */
	/*   puts(text); */
	free(p);
	return 1;
      }
      else{
        e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break;
      
    case DEBUT_HEXA:
      if( isxdigit(c) ) etat=HEXA;
      else{
        e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	free(p);
	return 1;
      }
      break;

    case HEXA:
      if( isxdigit(c) ) etat=HEXA;
      else if(c=='\0'){
	e.nature="[HEXA]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu,e);
	/* printf("[HEXA \t]\t%s\n ",text); */
        /* puts(text); */
	free(p);
	return 1;
      }
      else{
       e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break;
      
    case DECIMAL:
      if( isdigit(c) ) etat=DECIMAL;
      else if(c=='\0'){
	e.nature="[DECIMAL]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu,e);
	/* printf("[DECIMAL \t]\t%s\n ",text); */
	/* puts(text); */
	/* printf("\n"); */
	free(p);
	return 1;
      }
      else{
        e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break;

    case LETTRE_MIN:
      if( isalnum(c) ) etat=SYMBOLE; /*Début d'une etiquette*/
      else if(c=='\0'){
	e.nature="[SYMBOLE]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu,e);	
	/*printf("[SYMBOLE\t]\t");*/
	/*puts(text);*/
	free(p);
	return 1;
      }
      else{
        e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break;

    case LETTRE_MAJ:
      if( isupper(c) ) etat=MNEMO; /*Début d'une mnémonique*/
      else if( isalnum(c) && !( isupper( c ) ) ) etat=SYMBOLE;
      else if(c=='\0'){
	e.nature="[MNEMO]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu,e);
	/* printf("[MNEMO\t]\t"); */
	/* puts(text); */
	free(p);
	return 1;
      }
      else{
        e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break;

    case MNEMO:
      if( isupper(c) ) etat=MNEMO;/*Début d'une mnémonique*/
     else if( isalnum(c) && !( isupper( c ) ) ) etat=SYMBOLE;
     else if(c=='\0'){
	e.nature="[MNEMO]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu,e);
	/* printf("[MNEMO\t]\t"); */
	/* puts(text); */
	free(p);
	return 1;
      }
      else{
        e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break;

    case SYMBOLE :
      if( isalnum(c) ) etat=SYMBOLE;/*Début d'une mnémonique*/
      else if(c=='\0'){
	e.nature="[SYMBOLE]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu,e);
	/* printf("[SYMBOLE\t]\t"); */
	/* puts(text); */
	free(p);
	return 1;
      }
      else if(c==':') etat= ETIQUETTE;
      else{
        e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break;

    case ETIQUETTE:
      if(c=='\0'){
	e.nature="[ETIQUETTE]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu,e);	
	/* printf("[ETIQUETTE\t]\t"); */
	/* puts(text); */
	free(p);
	return 1;
      }
      else{
        e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break;

    case REGISTRE:
      if( isalnum(c) ) etat= REGISTRE;
      else if(c=='\0'){
	e.nature="[REGISTRE]";
	e.val_lex=text;
	e.ligne=l;
        enfiler(flu,e);	
	/* printf("[REGISTRE\t]\t"); */
	/* puts(text); */
	free(p);
	return 1;
      }
      else{
        e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
	printf("Erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break;

    case POINT:
      if( isalpha(c) ) etat= DIRECTIVE;
      else if(c=='\0'){
	e.nature="[PONCTUATION]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu,e);
	/* printf("[PONCTUATION\t]\t"); */
        /* puts(text);	 */
	free(p);
	return 1;
      }
      else{
        e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break;
      
    case DIRECTIVE:
     if( isalpha(c) ) etat= DIRECTIVE;
      else if(c=='\0'){
	e.nature="[DIRECTIVE]";
	e.val_lex=text;
	e.ligne=l;
	enfiler(flu,e);
	/* printf("[DIRECTIVE\t]\t"); */
	/* puts(text); */
	free(p);
	return 1;
      }
      else{
        e.nature="[ERREUR]";
	e.val_lex=text;
	e.ligne=l;
        printf("erreur de caractere\n");
	enfiler(flu, e);
	free(p);
	return 1;
      }
      break; 
      
      
    default:
      printf("erreur de programmation\n");
      free(p);
      return 0;
      break;
    }
    i++;
  }
  return 1;
  free(p);
}
