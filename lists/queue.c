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
  if (queue->primero == NULL)
    return NULL;
  void *dato = dnodo_dato(queue->ultimo);
  queue->ultimo = dnodo_ant(queue->ultimo);
  return dato;
}

void queue_destruir(Queue queue) {
  dlist_destruir(queue->primero, NULL);
  free(queue);
}
