/**
 * @file lex.h
 * @author Antoine CAMUS
 * @brief Lexem-related stuff.
 *
 * Contains lexem types definitions, some low-level syntax error codes,
 * the lexem structure definition and the associated prototypes.
 */

#ifndef _LEX_FILE_H_
#define _LEX_FILE_H_

#include <stdio.h>
#include "file.h"


/* Définition des etats */
enum {INIT, DECIMAL_ZERO, DEBUT_HEXA, HEXA, LETTRE_MIN, LETTRE_MAJ, DECIMAL, REGISTRE, SYMBOLE, MNEMO, ETIQUETTE, DIRECTIVE, POINT} ;


void	lex_read_line( char *, int, File *);

void	lex_load_file( char *, unsigned int * );

int     machine_etat( char * text, int nredline, File * flu );

char* 	getNextToken( char** , char* );

char*   newGetNextToken(char** token, char* current_line, int nline2, File * fligne );


#endif /*_LEX_H_*/ 
