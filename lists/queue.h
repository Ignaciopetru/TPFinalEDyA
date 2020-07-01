#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

// Tipo de la queue
typedef struct _Queue* Queue;

// queue_new crea una nueva queue.
Queue queue_new ();

// queue_isEmpty devuelve 1 si la queue esta vacia, 0 si no.
int queue_isEmpty (Queue);

// queue_agregar agrega un elemento al final de la queue.
void queue_agregar (Queue, void*);

// queue_sacar devuelve y elimina de la queue, el primer elemento de la misma.
void *queue_sacar (Queue);

// queue_destruir libera la memoria utilizada en la queue.
void queue_destruir (Queue);

#endif
