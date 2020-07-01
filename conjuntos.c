#include <stdio.h>
#include <stdlib.h>
#include "conjuntos.h"

// tipo = 0 -> extension
// tipo = 1 -> compresion
// tipo = -1 -> error

Conjunto crear_conjunto(int tipo, int *dato, int largo) {
  if (tipo == 0) {
    ConjExtension *conjuntoE = malloc(sizeof(ConjExtension));
    conjuntoE->cantidad = largo;
    conjuntoE->elementos = dato;
    Conjunto conjunto;
    conjunto.tipo = 0;
    conjunto.conj_extension = conjuntoE;
    conjunto.conj_compresion = NULL;
    return conjunto;
  }
  if (tipo == 1){
    ConjCompresion *conjuntoC = malloc(sizeof(ConjCompresion));
    conjuntoC->inicio = dato[0];
    conjuntoC->final = dato[1];
    free(dato);
    Conjunto conjunto;
    conjunto.tipo = 1;
    conjunto.conj_compresion = conjuntoC;
    conjunto.conj_extension = NULL;
    return conjunto;
  }
  Conjunto conjunto;
  conjunto.tipo = -1;
  return conjunto;
}

void liberar_conjunto(Conjunto conjunto) {
  if (conjunto.tipo == 0) {
    free(conjunto.conj_extension->elementos);
    free(conjunto.conj_extension);
    return;
  }
  if(conjunto.tipo == 1)
    free(conjunto.conj_compresion);
  else {
    printf("Intento de liberar estructura invalida");
    return;
  }
}

void mostrar_conjunto(Conjunto conjunto) {
 if (conjunto.tipo == 0) {
    for (int i = 0; i < conjunto.conj_extension->cantidad; i++) {
      printf("%d, ", conjunto.conj_extension->elementos[i]);
    }
    return;
  }
  if(conjunto.tipo == 1)
    printf("%d:%d", conjunto.conj_compresion->inicio, conjunto.conj_compresion->final);
  else {
    printf("Intento de mostrar estructura invalida");
    return;
  }
}


Conjunto unir_conjuntosExtension (Conjunto a, Conjunto b, Conjunto C) {
  Conjunto final;
  final.tipo = 0;
  final.conj_compresion = NULL;

  int * elementos = malloc(sizeof(int) * a.conj_extension->cantidad + b.conj_extension->cantidad);
  int i;
  for (i = 0; i < a.conj_extension->cantidad; i++) {
    elementos[i] = a.conj_extension->elementos;
  }
  for (int j = 0; j < b.conj_extension; j++) {
    elementos[i] = b.conj_extension->elementos;
    i++;
  }
  free(a.conj_extension->elementos);
  a.conj_extension->elementos = elementos;

}



