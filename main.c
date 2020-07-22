#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "interfaz.h"
#include "hash.h"

int main() {

  int salir = 1;
  HashTabla * tabla = hash_crear(10);
  printf("Interfaz 2.0\n");
  while (salir) {
    char buffer[CAPACIDAD];
    fgets (buffer, CAPACIDAD, stdin);
    if (strlen(buffer) == CAPACIDAD-1 ){
      printf("Largo excedido\n");
      scanf("%*[^\n]");
      scanf("%*c");
    } else {
    buffer[strlen(buffer)-1] = '\0';
    Tokens lista = token_lista_crear(buffer);
    salir = parser(tabla, lista);
    tokens_destruir(lista);
    }
  }
  hash_destuir(tabla);

  return 0;
}

