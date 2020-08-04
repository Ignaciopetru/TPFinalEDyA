#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"

HashTabla *hash_crear(int initial_size) {

  HashTabla *hash_tabla = malloc(sizeof(HashTabla));
  hash_tabla->size = initial_size;
  hash_tabla->tabla = calloc(initial_size, sizeof(HashDato));

  return hash_tabla;
}

// Funcion de hasheo.
int hash_obtener_key(int largoTabla, char *alias) {
  unsigned int hash = 5381;

  for (int i = 0; alias[i]; i++)
    hash = (33 * hash) + alias[i];
  // Puedo retornar un int, largoTabla es int, y hash % largoTabla <= largoTabla
  return hash % largoTabla;
}

int comparar(char *a1, char *a2){
  return strcmp(a1, a2) == 0;
}

void hash_insertar(HashTabla *tabla, char *alias, AVLTree conjunto) {
  int key;
  HashNodo *nodoNuevo = NULL;
  // Se obtienen la key del alias.
  key = hash_obtener_key(tabla->size, alias);

  for (HashNodo *aux = tabla->tabla[key].lista; aux != NULL; aux = aux->sig) {
    if (comparar(alias, aux->alias)){
      itree_destruir(aux->conjunto);
      aux->conjunto = conjunto;
      return;
    }
  }
  // se setea el nuevo nodo
  nodoNuevo = malloc(sizeof(HashNodo));
  // +1 para el terminador.
  nodoNuevo->alias = malloc(sizeof(char)*(strlen(alias) + 1));
  nodoNuevo->alias = strcpy(nodoNuevo->alias, alias);
  nodoNuevo->conjunto = conjunto;
  nodoNuevo->sig = tabla->tabla[key].lista;

  tabla->tabla[key].lista = nodoNuevo;
}

AVLTree hash_buscar(HashTabla *tabla, char *alias) {
  int key;
  HashNodo *nodo;

  key = hash_obtener_key(tabla->size, alias);
  // Busca en la lista.
  for (nodo = tabla->tabla[key].lista;
       nodo != NULL && comparar(alias, nodo->alias) != 1; nodo = nodo->sig);

  if (nodo == NULL)
    return NULL;

  return nodo->conjunto;
}

void hash_destuir(HashTabla *tabla) {
  HashNodo *nodo;
  HashNodo *nodoSig;

  for(int i = 0; i < tabla->size; i++) {
    nodo = tabla->tabla[i].lista;

    while(nodo != NULL) {
      nodoSig = nodo->sig;
      free(nodo->alias);
      itree_destruir(nodo->conjunto);
      free(nodo);
      nodo = nodoSig;
    }
  }

  free(tabla->tabla);
  free(tabla);
}

