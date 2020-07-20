#ifndef INTERFAZ_H_INCLUDED
#define INTERFAZ_H_INCLUDED
#include "hash.h"
#define CAPACIDAD 200

typedef struct _Token {
  int tipo;
  char *alias;
}Token;

typedef struct _Tokens{
  Token **palabras;
  int largo;
}Tokens;

Token *token_crear (char *);

Tokens token_lista_crear (char *);

void tokens_destruir(Tokens);

int parser(HashTabla *, Tokens);


#endif
