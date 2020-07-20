#include <stdio.h>
#include <stdlib.h>
#include "intervalo.h"


Intervalo intervalo_copiar(Intervalo intervalo) {
  Intervalo nuevo;
  nuevo.inicio = intervalo.inicio;
  nuevo.final = intervalo.final;
  return nuevo;
}

Intervalo intervalo_crear(int inicio, int final) {
  Intervalo nuevo;
  nuevo.inicio = inicio;
  nuevo.final = final;
  return nuevo;
}

int intervalo_interseccion(Intervalo datoArbol, Intervalo dato) {
  if (datoArbol.inicio <= dato.final && datoArbol.final >= dato.inicio)
    return 1;
  return 0;
}

void intervalo_imprimir(Intervalo intervalo) {
  if (intervalo.inicio == VACIO.inicio && intervalo.final == VACIO.final)
    printf("Ø");
  else if (intervalo.inicio == intervalo.final)
    printf("%d ", intervalo.inicio);
  else
    printf("%d:%d ", intervalo.inicio, intervalo.final);
}

int intervalo_validar(Intervalo intervalo) {
  return(intervalo.inicio <= intervalo.final && intervalo.inicio >= -INFINITO && intervalo.final <= +INFINITO);
}
