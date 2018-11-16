#ifndef _FILE_ANTE_H_
#define _FILE_ANTE_H_

typedef struct element {
	char* val_lex; 
	int ligne;
	char* nature; } Lex_t;

typedef struct maillon {
	Lex_t lexeme;
	struct maillon * suiv; } Maillon;

typedef Maillon * File;

File creer_file(void);
int file_vide(File);
File enfiler(Lex_t,File);
Lex_t defiler(File *);
void visualiser_file(File);

#endif
