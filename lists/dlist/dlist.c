#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"

struct _DNodo {
  void *dato;
  struct _DNodo *sig, *ant;
};

DList dlist_crear() {
  return NULL;
}

DNodo *dnodo_crear(void *dato, DNodo * ant, DNodo * sig) {
  // se pide memoria para la estructura nodo.
  DNodo *nuevo = malloc(sizeof(DNodo));
  // se establecen las variables.
  nuevo->ant = ant;
  nuevo->sig = sig;
  nuevo->dato = dato;
  return nuevo;
}

DList dnodo_agregar_inicio(DList lista, void *dato) {
  if (lista == NULL) {
    // Caso lista vacia.
    DNodo *nuevo = dnodo_crear(dato, NULL, NULL);
    return nuevo;
  } else {
    // Caso lista con elementos.
    // Creo un nuevo nodo
    DNodo *nuevo = dnodo_crear(dato, NULL, lista);
    // El antiguo primer elemento ahora es el segundo, asi que le asigno su ant
    lista->ant = nuevo;
    return nuevo;
  }
}

void dlist_destruir(DList lista, Visitante liberar) {
  DList proximo = lista;
  DList actual;
  for (; proximo != NULL;) {
    // Guardo un puntero al nodo actual, me muevo al siguiente y libero.
    actual = proximo;
    proximo = proximo->sig;
    if (liberar != NULL)
      liberar(actual->dato, NULL);
    free(actual);
  }
}

DList dlist_eliminar_primero(DList lista) {
  if(lista == NULL)
    return lista;
  DList nuevoInicio = lista->sig;
  free(lista);
  return nuevoInicio;
}

void *dnodo_dato(DNodo * nodo) {
  return nodo->dato;
}

DList dnodo_ant(DNodo * nodo) {
  return nodo->ant;
}
