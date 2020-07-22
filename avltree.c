#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avltree.h"
#include "lists/queue.h"
#include "lists/stack.h"

// Funciones auxiliares -------------------------------------------------------

int max(int a, int b) {
  if (a > b)
    return a;
  return b;
}
int min(int a, int b) {
  if (a < b)
    return a;
  return b;
}

int obtener_altura(AVLTree arbol) {
  if (arbol)
    return arbol->altura;
  return 0;
}

int obtener_balance(AVLTree arbol) {
  if (arbol)
    return obtener_altura(arbol->der) - obtener_altura(arbol->izq);
  return 0;
}

double obtener_mayorFinal(AVLTree arbol) {
  if (arbol->der == NULL || arbol->izq == NULL) {
    AVLTree temp = arbol->izq ? arbol->izq : arbol->der;
    // Sin hijos
    if (temp == NULL)
      return arbol->intervalo.final;
    else                        // Caso un hijo
      return max(arbol->intervalo.final, temp->mayorFinal);
  } else                        // Caso dos hijos
    return max(arbol->intervalo.final,
                      max(arbol->izq->mayorFinal,
                                 arbol->der->mayorFinal));
}

// Funciones de rotacion y balanceo. ------------------------------------------

AVLTree rotar_derecha(AVLTree arbol) {
  // Reservamos el izq del arbol
  AVLTree copArbolIzq = arbol->izq;
  // Realizamos la rotacion perse
  arbol->izq = copArbolIzq->der;
  copArbolIzq->der = arbol;
  // Recalculamos altura
  arbol->altura =
      max(obtener_altura(arbol->izq), obtener_altura(arbol->der)) + 1;
  arbol->mayorFinal = obtener_mayorFinal(arbol);
  copArbolIzq->altura =
      max(obtener_altura(copArbolIzq->izq),
          obtener_altura(copArbolIzq->der)) + 1;
  copArbolIzq->mayorFinal = obtener_mayorFinal(copArbolIzq);
  // Se retorna el nuevo primer nodo
  return copArbolIzq;
}

AVLTree rotar_izquierda(AVLTree arbol) {
  // Reservamos el der del arbol
  AVLTree copArbolDer = arbol->der;
  // Realizamos la rotacion perse
  arbol->der = copArbolDer->izq;
  copArbolDer->izq = arbol;
  // Recalculamos altura
  arbol->altura =
      max(obtener_altura(arbol->izq), obtener_altura(arbol->der)) + 1;
  arbol->mayorFinal = obtener_mayorFinal(arbol);
  copArbolDer->altura =
      max(obtener_altura(copArbolDer->izq),
          obtener_altura(copArbolDer->der)) + 1;
  copArbolDer->mayorFinal = obtener_mayorFinal(copArbolDer);
  // Se retorna el nuevo primer nodo
  return copArbolDer;
}

// Segun el valor del balance del nodo se decide como rebalancearlo.
AVLTree balancear(AVLTree arbol, int balance) {
  //Izquierda Izquierda
  if (balance < -1 && obtener_balance(arbol->izq) <= 0)
    return rotar_derecha(arbol);
  // Izquierda derecha
  if (balance < -1 && obtener_balance(arbol->izq) > 0) {
    arbol->izq = rotar_izquierda(arbol->izq);
    return rotar_derecha(arbol);
  }
  // Derecha derecha
  if (balance > 1 && obtener_balance(arbol->der) >= 0)
    return rotar_izquierda(arbol);
  // Derecha izquieda
  if (balance > 1 && obtener_balance(arbol->der) < 0) {
    arbol->der = rotar_derecha(arbol->der);
    return rotar_izquierda(arbol);
  }
  // Arbol balanceado
  return arbol;
}

// Funciones creacion ---------------------------------------------------------


AVLTree itree_crear() {
  return NULL;
}

AVLTree inodo_crear(Intervalo dato) {
  AVLTree nodo = malloc(sizeof(struct _AVLNodo));
  nodo->der = NULL;
  nodo->izq = NULL;
  nodo->altura = 1;
  nodo->mayorFinal = dato.final;
  nodo->intervalo = dato;
  return nodo;
}

AVLTree itree_duplicar(AVLTree arbol) {

  AVLTree copia = malloc(sizeof(AVLNodo));
  memcpy(copia, arbol, sizeof(AVLNodo));

  if(arbol->der != NULL)
    copia->der = itree_duplicar(arbol->der);
  else
    copia->der = NULL;
  if(arbol->izq != NULL)
    copia->izq = itree_duplicar(arbol->izq);
  else
    copia->izq = NULL;

  return copia;
}

AVLTree itree_insertar(AVLTree arbol, Intervalo dato) {
  // Si llegamos a nodo vacio insertamos nuestro nodo nuevo
  if (arbol == NULL) {
    return inodo_crear(dato);
  }
  // Buscamos la posicion que debe ocupar el nuevo nodo
  // segun BST, teniendo en cuenta ambos valores del intervalos.
  if (dato.inicio < arbol->intervalo.inicio)
    arbol->izq = itree_insertar(arbol->izq, dato);
  else if (dato.inicio > arbol->intervalo.inicio)
    arbol->der = itree_insertar(arbol->der, dato);
  else if (dato.final < arbol->intervalo.final)
    arbol->izq = itree_insertar(arbol->izq, dato);
  else if (dato.final > arbol->intervalo.final)
    arbol->der = itree_insertar(arbol->der, dato);
  // Caso en el que el nodo ya este en el arbol.
  else
    return arbol;
  // Recalculamos la altura de cada nuevo nodo.
  arbol->altura =
      1 + max(obtener_altura(arbol->izq), obtener_altura(arbol->der));
  arbol->mayorFinal = obtener_mayorFinal(arbol);
  // Calculamos el valor de balance de el nodo y procedemos a balancear.
  int balance = obtener_balance(arbol);
  return balancear(arbol, balance);
}

AVLTree itree_insertar_disjutos (AVLTree arbol, Intervalo intervalo) {

  AVLTree interseccion = itree_intersecar(arbol, intervalo_crear(intervalo.inicio - 1, intervalo.final + 1));
  while (interseccion != NULL) {
    intervalo.inicio = min(intervalo.inicio, interseccion->intervalo.inicio);
    intervalo.final = max(intervalo.final, interseccion->intervalo.final);
    arbol = itree_eliminar(arbol, interseccion->intervalo);
    interseccion = itree_intersecar(arbol, intervalo_crear(intervalo.inicio - 1, intervalo.final + 1));
  }
  return itree_insertar(arbol, intervalo);
}

AVLTree itree_intersecar(AVLTree arbol, Intervalo dato) {
  // Si llegue a NULL, retorno
  if (arbol == NULL)
    return arbol;
  // Si se encontro interseccion, devuelvo ese nodo
  if (intervalo_interseccion(arbol->intervalo, dato))
    return arbol;
  // Si no se logro intersecar, el nodo a intersecar esta completamente
  // a la izquierda o completamente a la derecha.

  // Primer caso:
  // si el intervalo a buscar es completamente menor que el intervalo a
  // del nodo, la unica posibilidad es que la interseccion orcurra en el
  // subarbol izquierdo.

  if (dato.final < arbol->intervalo.inicio) {
    // Si el mayor de los descendientes de la izquierda es mayor que el dato,
    // hago recursion ya que puede ser que haya interseccion en el mismo sub
    // arbol.
    if (arbol->izq != NULL && arbol->izq->mayorFinal >= dato.inicio)
      return itree_intersecar(arbol->izq, dato);
    // Si es menor, no hay forma que haya interseccion.
    else
      return NULL;
  }

  // Segundo caso:
  // si el intervalo a buscar es completamente mayor que el intervalo a
  // del nodo, verificamos dos casos particulares:

  // Primer subcaso:
  // Si el mayor de los valores finales del subarbol izquierdo es mayor que
  // el inicio del intervalo a intersecar, se sabe que si o si hay interseccion
  // a la izquierda.

  if (arbol->izq != NULL && arbol->izq->mayorFinal >= dato.inicio)
    return itree_intersecar(arbol->izq, dato);
  // Segundo subcaso:
  // la interseccion solo puede llegar a ocurrir a la derecha
  else
  // Antes de realizar la recursion, chequeamos si siquiera es posible
  // la interseccion con el dato.
    // Si el mayor del arbol es menor que el dato
    // ya se que no habra interseccion
  if (arbol->mayorFinal < dato.inicio)
    return NULL;
  // sino, finalmente hago recursion por la derecha
  else
    return itree_intersecar(arbol->der, dato);
}

AVLTree itree_copiar_agregar(AVLTree origen, AVLTree destino) {
  destino = itree_insertar_disjutos(destino, intervalo_copiar(origen->intervalo));
  return destino;
}

AVLTree itree_union(AVLTree a, AVLTree b) {
  AVLTree arbol = itree_crear();
  if (a->intervalo.inicio == VACIO.inicio && a->intervalo.final == VACIO.final)
    return itree_duplicar(b);
  if (b->intervalo.inicio == VACIO.inicio && b->intervalo.final == VACIO.final)
    return itree_duplicar(a);

  if(a->altura > b->altura) {
    arbol = itree_duplicar(a);
    arbol = itree_recorrer_dfs(b, itree_copiar_agregar, arbol);
    return arbol;
  }
  arbol = itree_duplicar(b);
  arbol = itree_recorrer_dfs(a, itree_copiar_agregar, arbol);
  return arbol;
}

AVLTree itree_interseccion(AVLTree a, AVLTree b) {
  if ((a->intervalo.inicio == VACIO.inicio && a->intervalo.final == VACIO.final) || (b->intervalo.inicio == VACIO.inicio && b->intervalo.final == VACIO.final))
    return itree_insertar(NULL, VACIO);

  AVLTree resultado = itree_crear();

  Stack stack = stack_new();
  stack_push(stack, a);

  while (!stack_isEmpty(stack)) {
    AVLTree nodo = stack_top(stack);
    stack_pop(stack);

    AVLTree nodoInterseccion = itree_intersecar(b, nodo->intervalo);
    if (nodoInterseccion) {
      Intervalo interseccion = nodoInterseccion->intervalo;
      resultado = itree_insertar_disjutos(resultado, intervalo_crear(max(interseccion.inicio, nodo->intervalo.inicio), min(interseccion.final, nodo->intervalo.final)));
    }

    if (nodo->izq != NULL)
      stack_push(stack, nodo->izq);
    if (nodo->der != NULL)
      stack_push(stack, nodo->der);
  }
  stack_destruir(stack);
  if (resultado == NULL)
    resultado = itree_insertar(resultado, VACIO);

  return resultado;
}

AVLTree itree_complemento(AVLTree conjunto) {
  // Caso conjunto vacio.
  if (conjunto->intervalo.inicio == VACIO.inicio && conjunto->intervalo.final == VACIO.final)
    return itree_insertar(NULL, intervalo_crear(-INFINITO, INFINITO));
  // Caso universo.
  if (conjunto->intervalo.inicio == -INFINITO && conjunto->intervalo.final == INFINITO)
    return itree_insertar(NULL, intervalo_crear(1, -1));

  AVLTree resultado = itree_crear();
  Intervalo anterior = intervalo_crear(0, -INFINITO - 1);

  Stack stack = stack_new();
  AVLTree actual = conjunto;

  while(actual != NULL || stack_isEmpty(stack) == 0) {
    while (actual) {
    stack_push(stack, actual);
    actual = actual->izq;
    }

    AVLTree temp = stack_top(stack);
    stack_pop(stack);
    actual = temp->der;

    resultado = itree_insertar_disjutos(resultado, intervalo_crear(anterior.final + 1, temp->intervalo.inicio - 1));
    anterior = temp->intervalo;
  }
  stack_destruir(stack);
  resultado = itree_insertar(resultado, intervalo_crear(anterior.final + 1, INFINITO));

  return resultado;

}

AVLTree itree_resta(AVLTree a, AVLTree b) {
  AVLTree complemento2 = itree_complemento(b);
  AVLTree resultado = itree_interseccion(a, complemento2);
  itree_destruir(complemento2);
  return resultado;
}

// Funciones destrucción. -----------------------------------------------------

AVLTree itree_eliminar(AVLTree arbol, Intervalo dato) {
  if (arbol == NULL) {
    printf("No existe el nodo a destruir\n");
    return arbol;
  }
  // Se hubica el nodo a eliminar.
  if (dato.inicio < arbol->intervalo.inicio)
    arbol->izq = itree_eliminar(arbol->izq, dato);
  else if (dato.inicio > arbol->intervalo.inicio)
    arbol->der = itree_eliminar(arbol->der, dato);
  else if (dato.final < arbol->intervalo.final)
    arbol->izq = itree_eliminar(arbol->izq, dato);
  else if (dato.final > arbol->intervalo.final)
    arbol->der = itree_eliminar(arbol->der, dato);
  else {
    if (arbol->der == NULL || arbol->izq == NULL) {
      AVLTree temp = arbol->izq ? arbol->izq : arbol->der;
      // Sin hijos
      if (temp == NULL) {
        temp = arbol;
        free(temp);
        arbol = NULL;
      } else {// Caso un hijo

        arbol->intervalo = temp->intervalo;
        arbol->izq = temp->izq;
        arbol->der = temp->der;
        free(temp);

      }
    } else {  // Caso dos hijos
      // Busco el nodo menor del hijo derecho
      AVLTree actual = arbol->der;
      while (actual->izq != NULL)
        actual = actual->izq;
      // Copio los datos del nodo encontrado y borro ese nodo
      arbol->intervalo = actual->intervalo;
      // bandera = 1 para no eliminar el intervalo al encontrar "actual"
      arbol->der = itree_eliminar(arbol->der, actual->intervalo);
    }
  }
  // Si no quedan nodos tras la eliminacion (caso inicial no hijos), retornamos
  if (arbol == NULL)
    return arbol;
  // Actualizo la altura si es necesaria
  arbol->altura =
      1 + max(obtener_altura(arbol->der), obtener_altura(arbol->izq));
  arbol->mayorFinal = obtener_mayorFinal(arbol);
  // Calculo el balance del nodo
  int balance = obtener_balance(arbol);

  // Retorno el arbol tras balancearlo
  return balancear(arbol, balance);
}

// Segundo argumento debe ser NULL

void itree_destruir(AVLTree arbol) {
  if (arbol) {
    itree_destruir(arbol->der);
    itree_destruir(arbol->izq);
  }
  free(arbol);

}

// Funciones recorrer

void inodo_imprimir(AVLTree nodo) {
  intervalo_imprimir(nodo->intervalo);
}



AVLTree itree_recorrer_dfs(AVLTree arbol, Visitante visitante, AVLTree puntero) {
  Stack stack = stack_new();
  if (arbol)
    stack_push(stack, arbol);
  while (!stack_isEmpty(stack)) {
    AVLTree nodo = stack_top(stack);
    stack_pop(stack);
    puntero = visitante(nodo, puntero);
    if (nodo->der != NULL)
      stack_push(stack, nodo->der);
    if (nodo->izq != NULL)
      stack_push(stack, nodo->izq);
  }
  stack_destruir(stack);
  return puntero;
}

AVLTree itree_recorrer_bfs(AVLTree arbol, Visitante visitante, AVLTree puntero) {
  Queue queue = queue_new();
  if(arbol)
    queue_agregar(queue, arbol);
  while (!queue_isEmpty(queue)) {
    AVLTree nodo = queue_sacar(queue);
    visitante(nodo, puntero);
    if (nodo->izq != NULL)
      queue_agregar(queue, nodo->izq);
    if (nodo->der != NULL)
      queue_agregar(queue, nodo->der);
  }
  queue_destruir(queue);
  return puntero;
}
