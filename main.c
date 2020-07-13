#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avltree.h"
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

int es_un_numero (char *palabra) {
  int retorno = 1;
  for(int i = 0; palabra[i]!= '\0'; i++) {
    if(isdigit(palabra[i]) == 0)
      retorno = 0;
  }
  return retorno;
}

// Hacer token_eliminar


Token *token_crear (char *palabra) {
  Token *token = malloc(sizeof(Token));
  token->alias = malloc(sizeof(char)*200);
  if (strcmp(palabra, "salir") == 0) {
    token->tipo = 1;
    token->alias = NULL;
    return token;
  } else if (strcmp(palabra, "imprimir") == 0) {
    token->tipo = 2;
    token->alias = NULL;
    return token;
  } else if (strcmp(palabra, "=") == 0) {
    token->tipo = 3;
    token->alias = NULL;
    return token;
  } else if (strcmp(palabra, "|") == 0) {
    token->tipo = 3;
    token->alias = NULL;
    return token;
  } else if (strcmp(palabra, "&") == 0) {
    token->tipo = 5;
    token->alias = NULL;
    return token;
  } else if (strcmp(palabra, "-") == 0) {
    token->tipo = 6;
    token->alias = NULL;
    return token;
  } else if (palabra[0] == '~') {
    token->tipo = 7;
    token->alias = strcpy(token->alias, palabra);
    return token;
  } else if (strcmp(palabra, "{x:") == 0) {
    token->tipo = 8;
    token->alias = NULL;
    return token;
  } else if (strcmp(palabra, "<=") == 0) {
    token->tipo = 9;
    token->alias = NULL;
    return token;
  } else if (strcmp(palabra, "x") == 0) {
    token->tipo = 10;
    token->alias = NULL;
    return token;
  } else if (palabra[strlen(palabra)] == '}') {
    token->tipo = 11;
    token->alias = strcpy(token->alias, palabra);
    return token;
  } else if (palabra[0] == '{') {
    // caso extension->
    token->tipo = 12;
    token->alias = palabra;
    return token;
  } else if (es_un_numero(palabra) == 1) {
    token->tipo = 13;
    token->alias = strcpy(token->alias, palabra);
    return token;
  } else {
    token->tipo = 14;
    token->alias = strcpy(token->alias, palabra);
    return token;
  }
}

Tokens token_lista_crear (const char *comando) {
  Tokens lista;
  lista.palabras = malloc(sizeof(Token*)*50);
  char * palabra = strtok(comando, " ");
  int i = 0;
   while( palabra != NULL ) {
      lista.palabras[i] = token_crear(palabra);
      palabra = strtok(NULL, " ");
      i++;
   }
  lista.largo = i;
  return lista;
}

int main() {
  char ff[20] = "798 = ~asd";
  Tokens hola = token_lista_crear(ff);
  for(int i = 0; i<hola.largo; i++){
    printf("%d, %s\n", hola.palabras[i]->tipo,hola.palabras[i]->alias);
  }
  return 0;
}

