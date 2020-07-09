#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED
#include "avltree.h"

typedef struct _HashNodo {
    char *alias;
    AVLTree conjunto;
    struct _HashNodo *sig;
} HashNodo;

typedef struct _HashDato{
    HashNodo *lista;
    int cantidad;
} HashDato;

typedef struct {
    HashDato *tabla;
    int max_colitions;
    int size;
    int used;
} HashTabla;

HashTabla *hash_crear(int);

HashTabla *hash_insertar(HashTabla *, char *, AVLTree);

AVLTree hash_buscar(HashTabla *, char *);

void hash_eliminar(HashTabla *, char *);

void hash_destuir(HashTabla *);

#endif // HASH_H_INCLUDED
