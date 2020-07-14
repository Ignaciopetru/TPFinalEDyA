#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "avltree.h"
#include "hash.h"
#define CAPACIDAD 200

typedef struct _Token {
  int tipo;
  char *alias;
}Token;

typedef struct _Tokens{
  Token **palabras;
  int largo;
}Tokens;

int es_un_numero (char *palabra) {
  for(int i = 0; palabra[i]!= '\0'; i++) {
    if(isdigit(palabra[i]) == 0)
      return 0;
  }
  return 1;
}

int es_un_numero_con_caracter(char *palabra, char caracter) {
  for (int i = 0; palabra[i]!= '\0'; i++) {
    if (isdigit(palabra[i]) == 0) {
      if (palabra[i] == caracter && palabra[i+1] == '\0')
        return 1;
      else
        return 0;
    }
  }
  return 1;
}

int alias_validar (char *alias) {
  for(int i = 0; alias[i]!= '\0'; i++) {
    if(isalpha(alias[i]) == 0)
      return 0;
  }
  return 1;
}

Token *token_crear (char *palabra) {
  Token *token = malloc(sizeof(Token));
  token->alias = calloc(200,sizeof(char));
  if (strcmp(palabra, "salir") == 0) {
    token->tipo = 1;
    return token;
  } else if (strcmp(palabra, "imprimir") == 0) {
    token->tipo = 2;
    return token;
  } else if (strcmp(palabra, "=") == 0) {
    token->tipo = 3;
    return token;
  } else if (strcmp(palabra, "|") == 0) {
    token->tipo = 4;
    return token;
  } else if (strcmp(palabra, "&") == 0) {
    token->tipo = 5;
    return token;
  } else if (strcmp(palabra, "-") == 0) {
    token->tipo = 6;
    return token;
  } else if (palabra[0] == '~') {
    token->tipo = 7;
    token->alias = strcpy(token->alias, palabra + 1);
    return token;
  } else if (strcmp(palabra, "{x:") == 0) {
    token->tipo = 8;
    return token;
  } else if (strcmp(palabra, "<=") == 0) {
    token->tipo = 9;
    return token;
  } else if (strcmp(palabra, "x") == 0) {
    token->tipo = 10;
    return token;
  } else if (palabra[strlen(palabra)-1] == '}'&& es_un_numero_con_caracter(palabra, '}')) {
    token->tipo = 11;
    token->alias = strcpy(token->alias, palabra);
    token->alias[strlen(palabra)-1] = '\0';
    return token;
  } else if (palabra[0] == '{' && es_un_numero_con_caracter(palabra + 1, ',')) {
    token->tipo = 12;
    token->alias = strcpy(token->alias, palabra+1);
    token->alias[strlen(palabra)-2] = '\0';
    return token;
  } else if (es_un_numero(palabra) == 1) {
    token->tipo = 13;
    token->alias = strcpy(token->alias, palabra);
    return token;
  } else if (palabra[strlen(palabra)-1] == ',' && es_un_numero_con_caracter(palabra, ',')) {
    token->tipo = 14;
    token->alias = strcpy(token->alias, palabra);
    token->alias[strlen(palabra)-1] = '\0';
    return token;
  } else if (palabra[strlen(palabra)-1] == '}' && palabra[0] == '{') {
    token->tipo = 16;
    token->alias = strcpy(token->alias, palabra+1);
    token->alias[strlen(token->alias) - 1] = '\0';
    return token;
  } else {
    token->tipo = 15;
    token->alias = strcpy(token->alias, palabra);
    return token;
  }
}

Tokens token_lista_crear (char *comando) {
  Tokens lista;
  lista.palabras = calloc(500, sizeof(Token*));
  char *palabra = strtok(comando, " ");
  int i = 0;
   while( palabra != NULL ) {
      lista.palabras[i] = token_crear(palabra);
      palabra = strtok(NULL, " ");
      i++;
   }
  lista.largo = i;
  return lista;
}

void tokens_destruir(Tokens lista) {
  for(int i = 0; i < lista.largo; i++) {
    free(lista.palabras[i]->alias);
    free(lista.palabras[i]);
  }
  free(lista.palabras);
}

int parser(HashTabla *tabla, Tokens lista) {

    if (lista.largo == 1) {
      if (lista.palabras[0]->tipo == 1){
        printf("Saliendo\n");
        return 1;
      }
      else {
        printf("Comando invalido\n");
        return 0;
      }
    }

    if (lista.largo == 2) {
      if (lista.palabras[0]->tipo == 2 && lista.palabras[1]->tipo == 15){
        if(alias_validar(lista.palabras[1]->alias)) {
          printf("Imprimir alias\n");
          return 1;
        } else {
          printf("Alias invalido\n");
          return 1;
        }
      } else {
        printf("Comando invalido\n");
        return 1;
      }
    }

    if (lista.largo == 3) {
      if (lista.palabras[1]->tipo == 3 && lista.palabras[0]->tipo == 15) {
        if (alias_validar(lista.palabras[0]->alias)) {
          if (lista.palabras[2]->tipo == 7) {
            if (alias_validar(lista.palabras[2]->alias)) {
              // guardar complemento del 2do en el 1ro
              printf("Complemento\n");
              return 1;
            } else {
               printf("2do Alias invalido\n");
               return 1;
            }
          } else if (lista.palabras[2]->tipo == 16) {
            if (es_un_numero(lista.palabras[2]->alias)) {
              // crear conjunto de un elemento
              printf("Conjunto de un elemento\n");
              return 1;
            } else {
               printf("Conjunto invalido\n");
               return 1;
            }
          }
        } else {
          printf("1er Alias invalido\n");
          return 1;
        }
      } else {
        printf("Comando invalido\n");
        return 1;
      }
      printf("Comando invalido\n");
      return 1;
    }

    if (lista.largo == 5) {
      if (lista.palabras[0]->tipo == 15 && lista.palabras[1]->tipo == 3 && lista.palabras[2]->tipo == 15 && lista.palabras[3]->tipo > 3 && lista.palabras[3]->tipo < 7 && lista.palabras[4]->tipo == 15) {
        if (alias_validar(lista.palabras[0]->alias) && alias_validar(lista.palabras[2]->alias) && alias_validar(lista.palabras[4]->alias)) {
          switch (lista.palabras[3]->tipo) {
          case 4:{
            printf("Hacer union\n");
            break;
          }
          case 5:{
            printf("Hacer interseccion\n");
            break;
          }
          case 6:{
            printf("Hacer resta\n");
            break;
          }
          default:
            break;
          }
          return 1;
        } else {
          printf("Alias invalidos\n");
        }
      }
    }

    if (lista.largo == 8) {
      if (lista.palabras[0]->tipo == 15 && lista.palabras[1]->tipo == 3 && lista.palabras[2]->tipo == 8 && lista.palabras[3]->tipo == 13 && lista.palabras[4]->tipo == 9 && lista.palabras[5]->tipo == 10 && lista.palabras[6]->tipo == 9 && lista.palabras[7]->tipo == 11) {
        if (alias_validar(lista.palabras[0]->alias)) {
          printf("Crear conjunto: {%s, %s}", lista.palabras[3]->alias, lista.palabras[7]->alias);
          return 1;
        } else {
          printf("Alias invalido\n");
          return 1;
        }
      }
    }

    if (lista.palabras[0]->tipo == 15) {
      if (alias_validar(lista.palabras[0]->alias)) {
        printf("Crar conjunto\n");
        for (int i = 2; i < lista.largo; i++) {
          if (lista.palabras[i]->tipo == 11 ||lista.palabras[i]->tipo == 12 || lista.palabras[i]->tipo == 14) {
            printf("{%s, %s} ", lista.palabras[i]->alias, lista.palabras[i]->alias);
          } else {
            printf("Sintaxis erronea\n");
            // Eliminar el conjunto creado hasta el momento.
            return 1;
          }
        }
        printf("Conjunto finalizado y almacenado\n");
        return 1;
      } else {
        printf("Alias invalido\n");
        return 1;
      }

    }

}

int main() {
  char ff[200] = "asd' = {123, 2, 3}";
  fgets (ff, 200, stdin);
  ff[strlen(ff)-1] = '\0';
  Tokens hola = token_lista_crear(ff);
  for(int i = 0; i<hola.largo; i++) {
    printf("%d, %s, %d\n", hola.palabras[i]->tipo,hola.palabras[i]->alias, alias_validar(hola.palabras[i]->alias));
  }
  printf("%d", parser(NULL, hola));
  tokens_destruir(hola);
  return 0;
}

