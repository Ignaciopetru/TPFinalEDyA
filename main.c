#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avltree.h"
#include "hash.h"
#define CAPACIDAD 200

int entrada_validar(char * comando, HashTabla *tabla) {

  if (strcmp(comando, "salir\n") == 0){
    printf("Saliendo...\n");
    return 0;
  }
  char primerPalabra[8];
  int i;
  for (i = 0; comando[i] != ' ' && comando[i] != '\n' && i < 8; i++) {
    primerPalabra[i] = comando[i];
  }
  primerPalabra[i] = '\0';
  if (comando[i] == '\n') {
    printf("Comando invalido por falta de argumentos.\n");
    return 1;
  }
  if (comando[i] != ' ') {
    printf("Comando invalido, se exedio la cantidad de caracteres.\n");
    return 1;
  }
  if (strcmp(primerPalabra, "imprimir") == 0) {
    char alias[10];
    int c = 0;
    for(int j = i+1; comando[j] != '\n'; j++) {
      alias[c] = comando[j];
      c++;
    }
    alias[c] = '\0';
    // llamar a funcion impimir en el alias propuesto.
    AVLTree conjunto = hash_buscar(tabla, alias);
    if (conjunto)
      itree_recorrer_bfs(conjunto, intervalo_imprimir);
    else
      printf("No existe el alias");
    return 1;
  }

  // si luego del primer alias la linea coincide con la creacion de comando, continuamos leyendo.
  if (/*validar_alias_nuevo(primerPalabra) == 1 &&*/ comando[i+1] == '=' && comando[i+2] == ' ') {
    if (comando[i + 3] == '~'){
      printf("Calculo del complemento\n");
      return 1;
    } else if (comando[i + 3] == '{'){
      if(comando[i + 4] == 'x') {
        char numeroInicio[8], numeroFinal[8];
        int c = 0;
        i = i + 7;
        for(; comando[i] != '\n' && comando[i]!= ' '; i++, c++)
          numeroInicio[c] = comando[i];
        if (comando[i] == '\n') {
          printf("Comando mal escrito\n");
          return 1;
        }
        numeroInicio[c] = '\0';
        i = i +9;
        for(c = 0; comando[i] != '\n' && comando[i]!= '}'; i++, c++)
          numeroFinal[c] = comando[i];
        if (comando[i] != '}') {
          printf("Comando mal escrito\n");
          return 1;
        }
        numeroFinal[c] = '\0';
        printf("%s\n", primerPalabra);
        if(hash_buscar(tabla, &primerPalabra) != NULL) {
          hash_eliminar(tabla, &primerPalabra);
          char *aliasNuevo = malloc(sizeof(primerPalabra));
          aliasNuevo = strcpy(aliasNuevo, primerPalabra);
          AVLTree conjunto = itree_crear();
          conjunto = itree_insertar(conjunto, intervalo_crear(atoi(numeroInicio), atoi(numeroFinal)));
          hash_insertar(tabla, aliasNuevo, conjunto);
        } else {
          char *aliasNuevo = malloc(sizeof(primerPalabra));
          aliasNuevo = strcpy(aliasNuevo, primerPalabra);
          AVLTree conjunto = itree_crear();
          conjunto = itree_insertar(conjunto, intervalo_crear(atoi(numeroInicio), atoi(numeroFinal)));
          hash_insertar(tabla, aliasNuevo, conjunto);
        }
        printf("[%s, %s] insenrtado\n", numeroInicio, numeroFinal);
        return 1;

        printf("Compresion\n");
      } else {
          // Extencion
          printf("Extension\n");
      }

    } else {
      // Operaciones de alias
      char alias2[8];
      int c = 0;
      for(i = i + 3; comando[i] != ' ' && comando[i] != '\n' && c < 8; i++) {
        alias2[c] = comando[i];
        c++;
      }
      alias2[c] = '\0';

      if (comando[i] == '\n') {
        printf("Comando invalido por falta de argumentos.\n");
        return 1;
      }
      if (comando[i] != ' ') {
        printf("Comando invalido, se exedio la cantidad de caracteres.\n");
        return 1;
      }

      if (comando[i + 1] == '|' || comando[i + 1] == '&' || comando[i + 1] == '-') {
        char operador = comando[i + 1];
        char alias3[8];
        int c = 0;
        for(i = i + 3; comando[i] != ' ' && comando[i] != '\n' && c < 8; i++) {
          alias3[c] = comando[i];
          c++;
        }
        alias3[c] = '\0';

        if (comando[i] != '\n') {
          printf("Comando invalido, se exedio la cantidad de caracteres.\n");
          return 1;
        }
        if (comando[i] == ' ') {
          printf("Comando invalido.\n");
          return 1;
        }
        //ejecutar_operacion(alias, alias2, alias3, operador);
        printf("Ejecucion de operacion");
      }
    }


  } else {
    printf("Comando invalidos, gramatica incorrecta\n");
    return 1;
  }

  return 1;
}


/*
int main()
{
    HashTabla *hash_tabla;


    hash_tabla = hash_crear(10);
    char * dato = malloc(sizeof(char)*8);
    dato = strcpy(dato, "Banana");
    hash_tabla = hash_insertar(hash_tabla, dato, NULL);

    AVLTree hola;
    hola = 12;
    hola = hash_buscar(hash_tabla, dato);
    hash_eliminar(hash_tabla, dato);
    printf("%d", hola);
    hash_destuir(hash_tabla);


    return 0;
}
*/
int main() {
  int salida = 1;

  printf("Interfaz 2.0\n");
  HashTabla *tabla = hash_crear(10);
  while (salida) {
    char *comando = malloc(sizeof(char) * CAPACIDAD);
    // leemos con \n incluido

    fgets(comando, CAPACIDAD, stdin);
    // Si se excede la capacidad maxima queda caracteres en el buffer,
    // en ese caso limpiamos el buffer y notificamos el error.

    if (strlen(comando) == CAPACIDAD-1 ){
      scanf("%*[^\n]");
      scanf("%*c");
      printf("Se exedio el largo permitido para un comando\n");
    } else
      // a entrada_validar se le debe pasar ademas la tabla hash
      salida = entrada_validar(comando, tabla);
    free(comando);

  }
  return 0;
}
