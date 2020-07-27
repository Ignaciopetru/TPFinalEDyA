#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "dlist/dlist.h"

struct _Queue{
  DList primero;
  DList ultimo;
};

Queue queue_new() {
    Queue queue = malloc(sizeof(struct _Queue));
    queue->primero = dlist_crear();
    queue->ultimo = queue->primero;
    return queue;
}

int queue_isEmpty(Queue queue) {
  return queue->ultimo == NULL;
}

void queue_agregar(Queue queue, void *dato) {
  if (queue_isEmpty(queue)) {
    queue->primero = dnodo_agregar_inicio(queue->primero, dato);
    queue->ultimo = queue->primero;
  } else
    queue->primero = dnodo_agregar_inicio(queue->primero, dato);
}

void *queue_sacar(Queue queue) {
  // Queue vacia.
  if (queue->primero == NULL)
    return NULL;

  void *dato = dnodo_dato(queue->ultimo);
  DList anteUltimo = dnodo_ant(queue->ultimo);

  // Un solo elemento en la queue.
  if (anteUltimo == NULL) {
    dlist_destruir(queue->ultimo, NULL);
    queue->primero = NULL;
    queue->ultimo = NULL;
    return dato;
  }

  anteUltimo->sig = NULL;
  dlist_destruir(queue->ultimo, NULL);
  queue->ultimo = anteUltimo;

  return dato;
}

void queue_destruir(Queue queue) {
  dlist_destruir(queue->primero, NULL);
  free(queue);
}
