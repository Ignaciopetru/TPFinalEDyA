#ifndef INTERFAZ_H_INCLUDED
#define INTERFAZ_H_INCLUDED
#include "hash.h"
#define CAPACIDAD 1024

// 18 posibles tipos de token
enum tipos{alias, igual, corNumComa, numComa, numCor, dobleCor, corNumCor, notAlias, imp, salir, unio, inter, resta, corX, num, x, menorIgual, error};

typedef struct _Token {
  int tipo;
  char *alias;
  int numero;
}Token;

typedef struct _Tokens{
  Token *palabras;
  int largo;
}Tokens;

Token token_crear (char *);

Tokens token_lista_crear (char *);

void tokens_destruir(Tokens);

int parser(HashTabla *, Tokens);


#endif
