#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "hash.h"
#include "lists/dlist/dlist.h"


HashTabla *hash_crear(int initial_size) {
    HashTabla *hash_tabla = malloc(sizeof(HashTabla));

    //memset(hash_tabla, 0, sizeof(hash_tabla));

    hash_tabla->max_colitions = 0;
    hash_tabla->size = initial_size;
    hash_tabla->used = 0;

    hash_tabla->tabla = malloc(sizeof(HashDato)*initial_size);

    //memset(hash_tabla->tabla, 0, sizeof(HashDato)*initial_size);

    return hash_tabla;
}


int hash_hasheo(char *alias) {
    unsigned long hash = 5381;
     int c;

     while (c = *alias++)
         hash = ((hash << 5) + hash) + c;

     return hash;
}

HashTabla *hash_insertar(HashTabla *tabla, char *alias, AVLTree conjunto) {
    int key;
    HashNodo *nodoNuevo;

    key = hash_hasheo(alias) % tabla->size;


    for (HashNodo *aux = tabla->tabla[key].lista; aux != NULL; aux = aux->sig) {
      if (strcmp(alias, aux->alias) == 0) {
        itree_destruir(aux->conjunto);
        aux->conjunto = conjunto;
        return tabla;
      }
    }

    // se setea el nuevo nodo
    nodoNuevo = malloc(sizeof(HashNodo));
    nodoNuevo->alias = malloc(sizeof(char)*strlen(alias));
    nodoNuevo->alias = strcpy(nodoNuevo->alias, alias);
    nodoNuevo->sig = tabla->tabla[key].lista;
    nodoNuevo->conjunto = conjunto;

    tabla->tabla[key].lista = nodoNuevo;
    tabla->tabla[key].cantidad++;

    if(tabla->max_colitions < tabla->tabla[key].cantidad)
        tabla->max_colitions = tabla->tabla[key].cantidad;

    return tabla;
}

int comparar(char*a1, char*a2){
  return strcmp(a1, a2) == 0;
}

AVLTree hash_buscar(HashTabla *tabla, char *alias) {
    int key;
    HashNodo *nodo;

    key = hash_hasheo(alias) % tabla->size;

    // Busca en la lista.
    for(nodo = tabla->tabla[key].lista; nodo != NULL && comparar(alias, nodo->alias)!= 1; nodo = nodo->sig);

    if(nodo == NULL)
        return NULL;
    return nodo->conjunto;
}

void hash_eliminar(HashTabla *tabla, char *alias) {
    int key;
    HashNodo *nodo;

    key = hash_hasheo(alias) % tabla->size;
    nodo = tabla->tabla[key].lista;

    for(nodo = tabla->tabla[key].lista; nodo != NULL && comparar(alias, nodo->alias)!= 1; nodo = nodo->sig);

    if(nodo == NULL) {
        return;
    }
    if (nodo == tabla->tabla[key].lista) {
      tabla->tabla[key].lista = nodo->sig;
    }

    free(nodo->alias);
    itree_destruir(nodo->conjunto);
    free(nodo);
}

void hash_destuir(HashTabla *tabla) {
    HashNodo *nodo;
    HashNodo *nodoSig;

    for(int i = 0; i<tabla->size; i++) {
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

