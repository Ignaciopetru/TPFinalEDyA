#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED
#include "intervalo.h"

typedef struct _AVLNodo {
    Intervalo intervalo;
    int altura;
    int mayorFinal;
    struct _AVLNodo *der;
    struct _AVLNodo *izq;
}AVLNodo;

  typedef struct _AVLNodo * AVLTree;

void inorder (AVLTree);

typedef AVLTree (*Visitante) (AVLTree, AVLTree);

// itree_crear inicializa el nuevo arbol de intervalos.
AVLTree itree_crear();

// itree_destruir libera toda la memoria utilizada en el arbol.
void itree_destruir(AVLTree);

// itree_insertar inserta un nuevo intervalo en el arbol,
//manteniendo el balance.
AVLTree itree_insertar(AVLTree, Intervalo);

// itree_intersecar busca en el arbol si un intervalo dado tiene
//interseccion con algun nodo.
AVLTree itree_intersecar(AVLTree, Intervalo);

// itree_eliminar elimina el nodo que contenga el intervalo dado.
AVLTree itree_eliminar(AVLTree, Intervalo);

// itree_recorrer_dfs recorre el arbol de la forma dfs.
AVLTree itree_recorrer_dfs(AVLTree, Visitante, AVLTree);

// itree_recorrer_dfs recorre el arbol de la forma bfs.
AVLTree itree_recorrer_bfs(AVLTree, Visitante, AVLTree);

// intervalo_imprimir (visitante) imprime los valores de un intervalo dado.
void inodo_imprimir(AVLTree);

// itree_insertar_disjutos inserta un intervalo en el arbol, pero de forma que
// no tenga interseccion con ningun otro intervalo.
AVLTree itree_insertar_disjutos (AVLTree, Intervalo);

// itree_union toma dos arboles y retorna uno que es la union.
AVLTree itree_union(AVLTree, AVLTree);

// itree_interseccion toma dos arboles y retorna uno que es la interseccion.
AVLTree itree_interseccion(AVLTree, AVLTree);

// itree_complemento toma dos arboles y retorna su complemento.
AVLTree itree_complemento(AVLTree);

// itree_resta toma dos arboles y retorna uno que es la resta.
AVLTree itree_resta(AVLTree, AVLTree);

#endif
