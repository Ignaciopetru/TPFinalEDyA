#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avltree.h"
#include "lists/queue.h"
#include "lists/stack.h"

// Funciones auxiliares --------------------------------------------------------

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

int obtener_mayorFinal(AVLTree arbol) {
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

// Funciones de rotacion y balanceo --------------------------------------------


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

AVLTree inodo_crear(Intervalo intervalo) {
  AVLTree nodo = malloc(sizeof(struct _AVLNodo));
  nodo->der = NULL;
  nodo->izq = NULL;
  nodo->altura = 1;
  nodo->mayorFinal = intervalo.final;
  nodo->intervalo = intervalo;
  return nodo;
}

// Devuelve un nodo identico al argumento.
AVLTree itree_duplicar(AVLTree arbol) {
  if (arbol == NULL)
    return NULL;
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
  if (arbol->intervalo.inicio == VACIO.inicio && arbol->intervalo.final == VACIO.final) {
    itree_destruir(arbol);
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

// Inserta un intervalo, pero de forma de que nunca haya en el arbol intervalos
// con interseccion no nula.
AVLTree itree_insertar_disjutos (AVLTree arbol, Intervalo intervalo) {

  // Se chequea si el intervalo tiene interseccion con algun nodo del arbol.
  // Se agranda el intervalo, pues si hay dos intervalos contiguos me interesa
  // unirlos, de forma que la cantidad de nodos sea la menor posible.
  AVLTree interseccion = itree_intersecar(arbol,intervalo_crear(intervalo.inicio - 1, intervalo.final + 1));

  while (interseccion != NULL) {
    // Si se encuentra interseccion, se elimina ese nodo y se modifica el
    // intervalo a insertar de manera que contenga ambos.
    intervalo.inicio = min(intervalo.inicio, interseccion->intervalo.inicio);
    intervalo.final = max(intervalo.final, interseccion->intervalo.final);
    arbol = itree_eliminar(arbol, interseccion->intervalo);
    interseccion = itree_intersecar(arbol, intervalo_crear(intervalo.inicio - 1, intervalo.final + 1));
  }

  // Una vez eliminadas todas las intersecciones y modificado el intervalo,
  // se inserta en el arbol.
  return itree_insertar(arbol, intervalo);
}

// Nodo resultado de intersecar un nodo con un arbol.
AVLTree itree_intersecar(AVLTree arbol, Intervalo intervalo) {
  if (arbol == NULL)
    return arbol;

  if (intervalo_interseccion(arbol->intervalo, intervalo))
    return arbol;

  // Intervalo completamente a la izquierda
  if (intervalo.final < arbol->intervalo.inicio) {
    if (arbol->izq != NULL && arbol->izq->mayorFinal >= intervalo.inicio)
      return itree_intersecar(arbol->izq, intervalo);
    // Si es menor, no hay forma que haya interseccion.
    else
      return NULL;
  }
  // Intervalo es completamente mayor que el intervalo del nodo

  // Primer subcaso:
  // Si el mayor de los valores finales del subarbol izquierdo es mayor que
  // el inicio del intervalo a intersecar, se sabe que si o si hay interseccion
  // a la izquierda.

  if (arbol->izq != NULL && arbol->izq->mayorFinal >= intervalo.inicio)
    return itree_intersecar(arbol->izq, intervalo);
  // Segundo subcaso:
  // la interseccion solo puede llegar a ocurrir a la derecha
  else
  // Antes de realizar la recursion, chequeamos si siquiera es posible
  // la interseccion con el dato.
    // Si el mayor del arbol es menor que el dato
    // ya se que no habra interseccion
  if (arbol->mayorFinal < intervalo.inicio)
    return NULL;
  // sino, finalmente hago recursion por la derecha
  else
    return itree_intersecar(arbol->der, intervalo);
}

// Funcion visitante, para agregar los nodos de un arbol en otro.
// Se utiliza con alguna funcion de recorrido.
AVLTree itree_copiar_agregar(AVLTree origen, AVLTree destino) {
  destino = itree_insertar_disjutos(destino, origen->intervalo);
  return destino;
}

AVLTree itree_union(AVLTree a, AVLTree b) {

  AVLTree arbol;

  if (a->intervalo.inicio == VACIO.inicio && a->intervalo.final == VACIO.final)
    return itree_duplicar(b);
  if (b->intervalo.inicio == VACIO.inicio && b->intervalo.final == VACIO.final)
    return itree_duplicar(a);

  // Duplicar es menos costoso que insertar, duplico el arbol mas grande.
  if(a->altura > b->altura) {
    arbol = itree_duplicar(a);
    // Se agregan los intervalos del otro arbol (conjunto).
    arbol = itree_recorrer_dfs(b, itree_copiar_agregar, arbol);
    return arbol;
  }

  arbol = itree_duplicar(b);
  arbol = itree_recorrer_dfs(a, itree_copiar_agregar, arbol);
  return arbol;
}

// Devuelve un arbol con todos los intervalos interseccion de un intervalo.
AVLTree  itree_todas_las_intersecciones (Intervalo intervalo, AVLTree arbol) {

  AVLTree resultado = NULL;
  Stack stack = stack_new();
  if (arbol)
    stack_push(stack, arbol);

  while (!stack_isEmpty(stack)) {
    AVLTree interseccion = itree_intersecar(stack_top(stack), intervalo);
    stack_pop(stack);

    if (interseccion) {
      resultado  = itree_insertar(resultado, intervalo_valor_interseccion(interseccion->intervalo, intervalo));
      // Si un intervalo tiene interseccion con alguno del arbol, al ser
      // intervalos disjuntos, las otras posibles intersecciones estan en los
      // hijos del nodo interseccion.
      if (interseccion->izq)
        stack_push(stack, interseccion->izq);
      if (interseccion->der)
        stack_push(stack, interseccion->der);
    }
  }

  stack_destruir(stack);

  if (!resultado) {
    resultado = itree_insertar(resultado, VACIO);
  }

  return resultado;
}

AVLTree itree_interseccion(AVLTree a, AVLTree b) {

  // Caso conjuntos vacio.
  if ((a->intervalo.inicio == VACIO.inicio &&
       a->intervalo.final == VACIO.final) ||
      (b->intervalo.inicio == VACIO.inicio &&
       b->intervalo.final == VACIO.final))
    return itree_insertar(NULL, VACIO);

  AVLTree conjuntoMasGrande, conjuntoMasChico, resultado = itree_crear();

  // Combiene recorrer el arbol mas chico, pues intersecar un intervalo suele
  // ser menos costoso.
  if (a->altura > b->altura) {
    conjuntoMasGrande = a;
    conjuntoMasChico = b;
  } else {
    conjuntoMasGrande = b;
    conjuntoMasChico = a;
  }

  Stack stack = stack_new();
  stack_push(stack, conjuntoMasChico);

  while (!stack_isEmpty(stack)) {
    AVLTree nodo = stack_top(stack);
    stack_pop(stack);

    // Todas las intersecciones del intervalo actual.
    AVLTree todasInter = itree_todas_las_intersecciones(nodo->intervalo,
                                                        conjuntoMasGrande);

    // Se agregan al resultado final.
    resultado = itree_recorrer_dfs(todasInter, itree_copiar_agregar, resultado);
    itree_destruir(todasInter);

    if (nodo->izq != NULL)
      stack_push(stack, nodo->izq);
    if (nodo->der != NULL)
      stack_push(stack, nodo->der);
  }

  stack_destruir(stack);

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
  Intervalo anterior = intervalo_crear(0, -INFINITO);

  // Recorrido en forma inorder iterativo.
  // de esta forma, recorremos del menor intervalo al mayor.
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
    // Realizo el caso particular de -INFINITO, para no salir del rango minimo
    // de ints.
    if (anterior.final == -INFINITO)
      resultado = itree_insertar_disjutos(resultado, intervalo_crear(anterior.final, temp->intervalo.inicio - 1));
    resultado = itree_insertar_disjutos(resultado, intervalo_crear(anterior.final + 1, temp->intervalo.inicio - 1));
    anterior = temp->intervalo;
  }

  stack_destruir(stack);
  resultado = itree_insertar(resultado, intervalo_crear(anterior.final + 1, INFINITO));

  return resultado;

}

AVLTree itree_resta(AVLTree a, AVLTree b) {

  // Propiedad de la resta de conjuntos.
  // A - B = A & (~B)
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
      } else {
        // Caso un hijo
        arbol->intervalo = temp->intervalo;
        arbol->izq = temp->izq;
        arbol->der = temp->der;
        free(temp);
      }
    } else {
      // Caso dos hijos
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

void itree_destruir(AVLTree arbol) {
  if (arbol) {
    itree_destruir(arbol->der);
    itree_destruir(arbol->izq);
  }
  free(arbol);
}

// Funciones recorridos --------------------------------------------------------

void inodo_imprimir(AVLTree nodo) {
  intervalo_imprimir(nodo->intervalo);
}

// Se iguala el parametro puntero a la ejecucion de la funcion visitante sobre
// el nodo actual del arbol.
AVLTree itree_recorrer_dfs(AVLTree arbol, Visitante visitante, AVLTree puntero){
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

// Se iguala el parametro puntero a la ejecucion de la funcion visitante sobre
// el nodo actual del arbol.
AVLTree itree_recorrer_bfs(AVLTree arbol, Visitante visitante, AVLTree puntero){
  Queue queue = queue_new();
  if (arbol)
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
