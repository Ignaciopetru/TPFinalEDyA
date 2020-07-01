#ifndef LIST_H_INLCUDED
#define LIST_H_INLCUDED

// Estructura que representa un nodo en la lista enlazada,
// cuyo dato es una personta
typedef struct _DNodo DNodo;

// Tipo de la lista, representa el tipo de puntero.
typedef DNodo* DList;

// Tipo de la funcion destruir, para cada tipo de dato sera distinta.
typedef void (*Visitante) (void *, void*);

// Prototipos de funciones de manejo de la lista.

// dlist_crear devuleve la lista vacia.
DList dlist_crear();

// dlist_crear crea la estructura nodo, con los datos pasados en argumentos.
DNodo* dnodo_crear(void* , DNodo*, DNodo*);

// dnodo_crear agrega un nodo a la lista, con los datos determinados (persona).
DList dnodo_agregar_inicio(DList, void*);

// dlist_destruir se encarga de liberar la memoria dedicada a la lista.
void dlist_destruir (DList, Visitante);

// dlist_eliminar_primero eliminia el primer nodo de la lista.
DList dlist_eliminar_primero(DList);

// dnodo_dato devulve el dato almacenado en el nodo.
void *dnodo_dato(DNodo *);

// dnodo_ant devuleve el puntero al nodo anterior de un nodo.
DList dnodo_ant(DNodo *);



#endif
