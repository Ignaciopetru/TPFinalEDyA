#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED
#include "avltree.h"

// Nodo de la lista
typedef struct _HashNodo {
    char *alias;
    AVLTree conjunto;
    struct _HashNodo *sig;
} HashNodo;

// Cabeza de la lista
typedef struct _HashDato{
    HashNodo *lista;
    int cantidad;
} HashDato;

typedef struct {
    // lista de listas
    HashDato *tabla;
    int size;
} HashTabla;

// hash_crear inicializa la estructura hash.
HashTabla *hash_crear(int);

// hash_insertar inserta un conjunto en la tabla hash.
// Si ya existe lo reemplaza.
void hash_insertar(HashTabla *, char *, AVLTree);

// hash_buscar retorna el conjunto con dicho alias si esta almacenado, NULL sino.
AVLTree hash_buscar(HashTabla *, char *);

// hash_destuir libera la memoria reservada.
void hash_destuir(HashTabla *);

#endif
