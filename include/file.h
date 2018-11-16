#ifndef H_FILE
#define H_FILE

typedef struct Lex {
	char* val_lex; 
	int ligne;
	char* nature;} lex_t;

typedef struct Element Element;
struct Element{
	lex_t lexeme;
	Element* suivant;};

typedef struct File File;
struct File{
	Element *premier;};


File *initialiser();

void enfiler(File *file, lex_t lex);

lex_t defiler(File *file);

void afficherFile(File *file);

#endif
