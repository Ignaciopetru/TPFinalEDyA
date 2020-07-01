#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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
      return arbol->intervalo->final;
    else                        // Caso un hijo
      return max(arbol->intervalo->final, temp->mayorFinal);
  } else                        // Caso dos hijos
    return max(arbol->intervalo->final,
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
  // Se retorna el nuevo primer nodoz
  printf("Roto derecha\n");
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
  // Se retorna el nuevo primer nodo
  printf("Roto izquierda\n");
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

Intervalo *intervalo_copiar(Intervalo *intervalo) {
  Intervalo *nuevo = malloc(sizeof(Intervalo));
  nuevo->inicio = intervalo->inicio;
  nuevo->final = intervalo->final;
  return nuevo;
}

Intervalo *intervalo_crear(int inicio, int final) {
  Intervalo *nuevo = malloc(sizeof(Intervalo));
  nuevo->inicio = inicio;
  nuevo->final = final;
  return nuevo;
}

AVLTree itree_crear() {
  return NULL;
}

int inodo_interseccion(Intervalo * datoArbol, Intervalo * dato) {
  if (datoArbol->inicio <= dato->final && datoArbol->final >= dato->inicio)
    return 1;
  return 0;
}

AVLTree inodo_crear(Intervalo * dato) {
  AVLTree nodo = malloc(sizeof(struct _AVLNodo));
  nodo->der = NULL;
  nodo->izq = NULL;
  nodo->altura = 1;
  nodo->mayorFinal = dato->final;
  nodo->intervalo = dato;
  return nodo;
}

AVLTree itree_colisionDerecha(AVLTree arbol) {
  if(arbol->der == NULL)
    return arbol;
  Intervalo auxiliar;
  auxiliar.inicio = arbol->der->intervalo->inicio - 1;
  auxiliar.final = arbol->der->intervalo->final + 1;
  if (inodo_interseccion(&auxiliar, arbol->intervalo)) {
    arbol->intervalo->final = auxiliar.final - 1;
    arbol = itree_eliminar(arbol, arbol->der->intervalo, 0);
  }
  AVLTree iterador = itree_crear();
  for (; iterador != NULL; iterador = iterador->izq) {
    auxiliar.inicio = iterador->intervalo->inicio - 1;
    auxiliar.final = iterador->intervalo->final + 1;
    if (inodo_interseccion(&auxiliar, arbol->intervalo)) {
      arbol->intervalo->final = iterador->intervalo->final;
      arbol = itree_eliminar(arbol, iterador->intervalo, 0);
    }
  }
  return arbol;
}

AVLTree itree_colisionIzquierda(AVLTree arbol) {
  if(arbol->izq == NULL)
    return arbol;
  Intervalo auxiliar;
  auxiliar.inicio = arbol->izq->intervalo->inicio - 1;
  auxiliar.final = arbol->izq->intervalo->final + 1;
  if (inodo_interseccion(&auxiliar, arbol->intervalo)) {
    arbol->intervalo->inicio = auxiliar.inicio + 1;
    arbol = itree_eliminar(arbol, arbol->izq->intervalo,0);
  }
  AVLTree iterador = itree_crear();
  for (; iterador != NULL; iterador = iterador->der) {
    auxiliar.inicio = iterador->intervalo->inicio - 1;
    auxiliar.final = iterador->intervalo->final + 1;
    if (inodo_interseccion(&auxiliar, arbol->intervalo)) {
      arbol->intervalo->inicio = iterador->intervalo->inicio;
      arbol = itree_eliminar(arbol, iterador->intervalo,0);
    }
  }
  return arbol;
}


// Funciones comparacion. -----------------------------------------------------



AVLTree itree_insertar(AVLTree arbol, Intervalo * dato) {
  // Si llegamos a nodo vacio insertamos nuestro nodo nuevo
  if (arbol == NULL) {
    return inodo_crear(dato);
  }
  Intervalo auxiliar;
  auxiliar.inicio = arbol->intervalo->inicio - 1;
  auxiliar.final = arbol->intervalo->final + 1;
  if (inodo_interseccion(&auxiliar, dato)) {
    arbol->intervalo->inicio = min(arbol->intervalo->inicio, dato->inicio);
    arbol->intervalo->final = max(arbol->intervalo->final, dato->final);

    // Si el nuevo intervalo tiene inicio menor al del arbol hay dos opciones:
    if (min(arbol->intervalo->inicio, dato->inicio) == dato->inicio) {
      // si tambien sobresale por la derecha la colicion es doble
      if(max(arbol->intervalo->final, dato->final) == dato->final) {
        //colicion de ambos ladossss
      } else {
        // sino solo por la izquierda
        arbol = itree_colisionIzquierda(arbol);
      }
    // sino puede que sobresalga por la derecha
    } else if(max(arbol->intervalo->final, dato->final) == dato->final) {
      arbol = itree_colisionDerecha(arbol);
    }
    // sino el nuevo nodo esta contenido en el viejo
    free(dato);
    return arbol;
  }
  // Buscamos la posicion que debe ocupar el nuevo nodo
  // segun BST, teniendo en cuenta ambos valores del intervalos.
  if (dato->inicio < arbol->intervalo->inicio)
    arbol->izq = itree_insertar(arbol->izq, dato);
  else if (dato->inicio > arbol->intervalo->inicio)
    arbol->der = itree_insertar(arbol->der, dato);
  else if (dato->final < arbol->intervalo->final)
    arbol->izq = itree_insertar(arbol->izq, dato);
  else if (dato->final > arbol->intervalo->final)
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



AVLTree itree_intersecar(AVLTree arbol, Intervalo * dato) {
  // Si llegue a NULL, retorno
  if (arbol == NULL)
    return arbol;
  // Si se encontro interseccion, devuelvo ese nodo
  if (inodo_interseccion(arbol->intervalo, dato))
    return arbol;
  // Si no se logro intersecar, el nodo a intersecar esta completamente
  // a la izquierda o completamente a la derecha.

  // Primer caso:
  // si el intervalo a buscar es completamente menor que el intervalo a
  // del nodo, la unica posibilidad es que la interseccion orcurra en el
  // subarbol izquierdo.

  if (dato->final < arbol->intervalo->inicio) {
    // Si el mayor de los descendientes de la izquierda es mayor que el dato,
    // hago recursion ya que puede ser que haya interseccion en el mismo sub
    // arbol.
    if (arbol->izq != NULL && arbol->izq->mayorFinal >= dato->inicio)
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

  if (arbol->izq != NULL && arbol->izq->mayorFinal >= dato->inicio)
    return itree_intersecar(arbol->izq, dato);
  // Segundo subcaso:
  // la interseccion solo puede llegar a ocurrir a la derecha
  else
  // Antes de realizar la recursion, chequeamos si siquiera es posible
  // la interseccion con el dato.
    // Si el mayor del arbol es menor que el dato
    // ya se que no habra interseccion
  if (arbol->mayorFinal < dato->inicio)
    return NULL;
  // sino, finalmente hago recursion por la derecha
  else
    return itree_intersecar(arbol->der, dato);
}

AVLTree inodo_copiar(Intervalo * origen, AVLTree destino) {
  destino = itree_insertar(destino, intervalo_copiar(origen));
  return destino;
}

AVLTree itree_union(AVLTree a, AVLTree b) {
  AVLTree arbol = itree_crear();
  arbol = itree_recorrer_dfs(a, inodo_copiar, arbol);
  arbol = itree_recorrer_dfs(b, inodo_copiar, arbol);
  return arbol;
}

AVLTree itree_interseccion(AVLTree a, AVLTree b) {
  AVLTree resultado = itree_crear();

  Stack stack = stack_new();
  stack_push(stack, a);
  while (!stack_isEmpty(stack)) {
    AVLTree nodo = stack_top(stack);
    stack_pop(stack);
    if (nodo != NULL) {
      Intervalo * interseccion = itree_intersecar(b, nodo->intervalo)->intervalo;
      intervalo_imprimir(interseccion);
      resultado = itree_insertar(resultado, intervalo_crear(max(interseccion->inicio, nodo->intervalo->inicio), min(interseccion->final, nodo->intervalo->final)));
      stack_push(stack, nodo->der);
      stack_push(stack, nodo->izq);
    }
  }
  stack_destruir(stack);

  return resultado;
}

// Funciones destrucción. -----------------------------------------------------

void inodo_liberar(AVLTree arbol) {
  free(arbol->intervalo);
  free(arbol);
}

AVLTree itree_eliminar(AVLTree arbol, Intervalo * dato, int bandera) {
  if (arbol == NULL) {
    printf("No existe el nodo a destruir\n");
    return arbol;
  }
  // Se hubica el nodo a eliminar.
  if (dato->inicio < arbol->intervalo->inicio)
    arbol->izq = itree_eliminar(arbol->izq, dato, bandera);
  else if (dato->inicio > arbol->intervalo->inicio)
    arbol->der = itree_eliminar(arbol->der, dato, bandera);
  else if (dato->final < arbol->intervalo->final)
    arbol->izq = itree_eliminar(arbol->izq, dato, bandera);
  else if (dato->final > arbol->intervalo->final)
    arbol->der = itree_eliminar(arbol->der, dato, bandera);
  else {
    if (arbol->der == NULL || arbol->izq == NULL) {
      AVLTree temp = arbol->izq ? arbol->izq : arbol->der;
      // Sin hijos
      if (temp == NULL) {
        temp = arbol;
        // Caso directo, se libera todo
        if (bandera == 0)
          inodo_liberar(temp);
        // Caso venir de dos hijos. No se libera el intervalo
        if (bandera == 1)
          free(temp);
        arbol = NULL;
      } else {// Caso un hijo
        // Caso directo, se libera intervalo de raiz, compia datos hijo
        // libera nodo hijo sin liberar intervalo
        if (bandera == 0) {
          free(arbol->intervalo);
          arbol->intervalo = temp->intervalo;
          arbol->izq = temp->izq;
          arbol->der = temp->der;
          free(temp);
        }
        // Caso venir de dos hijos, igual anterior no se libera intervalo raiz
        // ya que ahora es la raiz de la llamada original de dos hijos.
        // swap
        if (bandera == 1) {
          arbol->intervalo = temp->intervalo;
          arbol->izq = temp->izq;
          arbol->der = temp->der;
          free(temp);
        }
      }
    } else {                    // Caso dos hijos
      // Busco el nodo menor del hijo derecho
      AVLTree actual = arbol->der;
      while (actual->izq != NULL)
        actual = actual->izq;
      // Copio los datos del nodo encontrado y borro ese nodo
      free(arbol->intervalo);
      arbol->intervalo = actual->intervalo;
      // bandera = 1 para no eliminar el intervalo al encontrar "actual"
      arbol->der = itree_eliminar(arbol->der, actual->intervalo, 1);
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

void itree_destruir(AVLTree arbol) {
  if (arbol != NULL) {
    itree_destruir(arbol->der);
    itree_destruir(arbol->izq);
    inodo_liberar(arbol);
  }
}

// Funciones recorrer

void intervalo_imprimir(Intervalo * intervalo) {
  printf("[%d, %d]\n", intervalo->inicio, intervalo->final);
}

void inorder(AVLTree arbol) {
  if (arbol == NULL)
    return;
  inorder(arbol->izq);
  printf("[%d, %d] - maximo: %d altura:%d\n", arbol->intervalo->inicio,
         arbol->intervalo->final, arbol->mayorFinal, arbol->altura);
  inorder(arbol->der);
}

AVLTree itree_recorrer_dfs(AVLTree arbol, Visitante visitante, AVLTree puntero) {
  Stack stack = stack_new();
  stack_push(stack, arbol);
  while (!stack_isEmpty(stack)) {
    AVLTree nodo = stack_top(stack);
    stack_pop(stack);
    if (nodo != NULL) {
      puntero = visitante(nodo->intervalo, puntero);
      stack_push(stack, nodo->der);
      stack_push(stack, nodo->izq);
    }
  }
  stack_destruir(stack);
  return puntero;
}

void itree_recorrer_bfs(AVLTree arbol, Visitante visitante) {
  Queue queue = queue_new();
  queue_agregar(queue, arbol);
  while (!queue_isEmpty(queue)) {
    AVLTree nodo = queue_sacar(queue);
    if (nodo != NULL) {
      visitante(nodo->intervalo, NULL);
      queue_agregar(queue, nodo->izq);
      queue_agregar(queue, nodo->der);
    }
  }
  queue_destruir(queue);
}
