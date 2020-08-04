#ifndef INTERFAZ_H_INCLUDED
#define INTERFAZ_H_INCLUDED
#include "hash.h"
// Tamanio de linea maximo
#define CAPACIDAD 1024

// 18 posibles tipos de token
enum Tipos{alias, igual, corNumComa, numComa, numCor, dobleCor, corNumCor,
  notAlias, imp, salir, unio, inter, resta, corX, num, x, menorIgual, error};

typedef struct _Token {
  int tipo;
  char *alias;
  int numero;
}Token;

typedef struct _Tokens{
  Token *palabras;
  int largo;
}Tokens;

// tokens_lista_crear toma una linea de texto
// y devuelve la lista de tokens asociada.
Tokens tokens_lista_crear (char *);

// tokens_destruir libera la memoria reservada.
void tokens_destruir(Tokens);

// ejecutar_comando toma una tabla hash y una estructura tokens y realiza la
// operacion correspondiente de ser posible. Retorna 0 si el comando es salir,
// en otro caso 0.
int ejecutar_comando(HashTabla *, Tokens);

#endif
