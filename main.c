#include <stdio.h>
#include <string.h>
#include "interfaz.h"
#include "hash.h"

int main() {

  int salir = 1;
  HashTabla * tabla = hash_crear(10);
  printf("Interfaz 2.0\n");

  while (salir) {
    char buffer[CAPACIDAD];
    fgets (buffer, CAPACIDAD, stdin);
    // limpiar excedente del buffer
    if (strlen(buffer) == CAPACIDAD-1 ){
      printf("Largo excedido\n");
      scanf("%*[^\n]");
      scanf("%*c");
    } else {
      buffer[strlen(buffer)-1] = '\0';
      Tokens lista = tokens_lista_crear(buffer);
      salir = ejecutar_comando(tabla, lista);
      tokens_destruir(lista);
    }
  }

  hash_destuir(tabla);

  return 0;
}

