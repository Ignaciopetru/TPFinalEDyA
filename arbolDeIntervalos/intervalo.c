#include <stdio.h>
#include "intervalo.h"

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
  else if (intervalo.inicio == -INFINITO && intervalo.final == INFINITO)
    printf("-inf:inf ");
  else if (intervalo.inicio == -INFINITO)
    printf("-inf:%d ", intervalo.final);
  else if (intervalo.final == INFINITO)
    printf("%d:inf ", intervalo.final);
  else
    printf("%d:%d ", intervalo.inicio, intervalo.final);
}

int intervalo_validar(Intervalo intervalo) {
  return (intervalo.inicio <= intervalo.final &&
          intervalo.inicio >= -INFINITO && intervalo.final <= +INFINITO);
}

Intervalo intervalo_valor_interseccion(Intervalo interA, Intervalo interB) {
  Intervalo resultado;
  resultado.inicio = max(interA.inicio, interB.inicio);
  resultado.final = min(interA.final, interB.final);
  return resultado;
}