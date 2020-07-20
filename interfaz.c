#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "interfaz.h"
#include "avltree.h"
#include "hash.h"
#include "intervalo.h"


int es_un_numero (char *palabra) {
  for(int i = 0; palabra[i]!= '\0'; i++) {
    if(isdigit(palabra[i]) == 0 && palabra[i] != '-')
      return 0;
  }
  return 1;
}

int es_un_numero_con_caracter(char *palabra, char caracter) {
  for (int i = 0; palabra[i]!= '\0'; i++) {
    if (isdigit(palabra[i]) == 0 && palabra[i] != '-') {
      if (palabra[i] == caracter && palabra[i+1] == '\0')
        return 1;
      else
        return 0;
    }
  }
  return 1;
}

int alias_validar_sintaxis (char *alias) {
  for(int i = 0; alias[i]!= '\0'; i++) {
    if(isalpha(alias[i]) == 0 && isdigit(alias[i]) == 0)
      return 0;
  }
  return 1;
}

// NULL si esta mal escrito o no existe, el arbol si existe
AVLTree alias_validar (HashTabla *tabla, char *alias) {
  for(int i = 0; alias[i]!= '\0'; i++) {
    if(isalpha(alias[i]) == 0 && isdigit(alias[i]) == 0)
      return NULL;
  }
  return hash_buscar(tabla, alias);
}

Token *token_crear (char *palabra) {
  Token *token = malloc(sizeof(Token));
  token->alias = calloc(CAPACIDAD,sizeof(char));
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
  } else if (palabra[strlen(palabra)-1] == '}' && es_un_numero_con_caracter(palabra, '}')) {
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
  } else if (strcmp(palabra, "{}") == 0) {
    token->tipo = 17;
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
  lista.palabras = calloc(CAPACIDAD, sizeof(Token*));
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
  switch (lista.largo) {
    case 0:
      return 1;
      break;
    case 1: {
      if (lista.palabras[0]->tipo == 1){
        printf("Saliendo\n");
        return 0;
      }
      else {
        printf("Comando invalido\n");
        return 1;
      }
    break;
    }
    case 2: {
      if (lista.palabras[0]->tipo == 2 && lista.palabras[1]->tipo == 15){
      AVLTree conjunto = alias_validar(tabla, lista.palabras[1]->alias);
      if (conjunto) {
          itree_recorrer_bfs(conjunto, (Visitante) inodo_imprimir, NULL);
          puts("");
          return 1;
      } else {
          printf("Alias invalido\n");
          return 1;
        }
      } else {
        printf("Comando invalido\n");
        return 1;
      }
      break;
    }
    case 3: {
      if (lista.palabras[1]->tipo == 3 && lista.palabras[0]->tipo == 15 && lista.palabras[2]->tipo == 7) {
      // puede no existir el conjunto.
      if (alias_validar_sintaxis(lista.palabras[0]->alias)) {
          // si el alias es correcto y existe
          AVLTree conjuntoComplemento = alias_validar(tabla, lista.palabras[2]->alias);
          if (conjuntoComplemento) {
            // insertar si encuentra el mismo elemento reemplaza.
            hash_insertar(tabla, lista.palabras[0]->alias, itree_complemento(conjuntoComplemento));
            return 1;
          } else {
             printf("2do Alias invalido\n");
             return 1;
          }
      } else {
          printf("1er alias invalido\n");
          return 1;
        }
      }
    break;
    }
    case 5: {
          if (lista.palabras[0]->tipo == 15 && lista.palabras[1]->tipo == 3 && lista.palabras[2]->tipo == 15 && lista.palabras[3]->tipo > 3 && lista.palabras[3]->tipo < 7 && lista.palabras[4]->tipo == 15) {
      AVLTree conjutoOperando1 = alias_validar(tabla, lista.palabras[2]->alias);
      AVLTree conjutoOperando2 = alias_validar(tabla, lista.palabras[4]->alias);
      if (alias_validar_sintaxis(lista.palabras[0]->alias) && conjutoOperando1 && conjutoOperando2) {
        switch (lista.palabras[3]->tipo) {
        case 4:{
          tabla = hash_insertar(tabla, lista.palabras[0]->alias, itree_union(conjutoOperando1, conjutoOperando2));
          break;
        }
        case 5:{
          tabla = hash_insertar(tabla, lista.palabras[0]->alias, itree_interseccion(conjutoOperando1, conjutoOperando2));
          break;
        }
        case 6:{
          tabla = hash_insertar(tabla, lista.palabras[0]->alias, itree_resta(conjutoOperando1, conjutoOperando2));
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
    break;
    }
    case 8: {
      if (lista.palabras[0]->tipo == 15 && lista.palabras[1]->tipo == 3 && lista.palabras[2]->tipo == 8 && lista.palabras[3]->tipo == 13 && lista.palabras[4]->tipo == 9 && lista.palabras[5]->tipo == 10 && lista.palabras[6]->tipo == 9 && lista.palabras[7]->tipo == 11) {
      if (alias_validar_sintaxis(lista.palabras[0]->alias)) {
          int inicio = atoi(lista.palabras[3]->alias);
          int final = atoi(lista.palabras[7]->alias);
          if (intervalo_validar(intervalo_crear(inicio, final))) {
            tabla = hash_insertar(tabla, lista.palabras[0]->alias, itree_insertar_disjutos(NULL, intervalo_crear(inicio, final)));
            return 1;
          } else {
            printf("Intervalo invalido\n");
            return 1;
          }
        }
      } else {
        printf("Alias invalido\n");
        return 1;
      }
    break;
    }
    default: {
      if (lista.palabras[0]->tipo == 15 && lista.largo > 2) {
        if (alias_validar_sintaxis(lista.palabras[0]->alias)) {

          AVLTree conjunto = itree_crear();
          if (lista.palabras[2]->tipo == 17 && lista.largo == 3) {
            conjunto = itree_insertar_disjutos(conjunto, intervalo_crear(1,-1));
          } else {

            if (lista.palabras[2]->tipo == 12) {
              int inicio = atoi(lista.palabras[2]->alias);
              // Primer elemento {num
              if (intervalo_validar(intervalo_crear(inicio, inicio))) {
                conjunto = itree_insertar_disjutos(conjunto, intervalo_crear(inicio, inicio));
              } else {
                printf("Numero fuera de rango\n");
                return 1;
              }
              // Elementos intermedios num,
              for (int i = 3; i < lista.largo - 1; i++) {
                if (lista.palabras[i]->tipo == 14) {
                  inicio = atoi(lista.palabras[i]->alias);
                  if (intervalo_validar(intervalo_crear(inicio, inicio))) {
                    conjunto = itree_insertar_disjutos(conjunto, intervalo_crear(inicio, inicio));
                  } else {
                    printf("Numero fuera de rango\n");
                    itree_destruir(conjunto);
                    return 1;
                  }
                } else {
                    printf("Sintaxis erronea\n");
                    itree_destruir(conjunto);
                    return 1;
                }
              }
              // Elemento final num}
              if (lista.palabras[lista.largo - 1]->tipo == 11) {
                inicio = atoi(lista.palabras[lista.largo - 1]->alias);
                if (intervalo_validar(intervalo_crear(inicio, inicio))) {
                  conjunto = itree_insertar_disjutos(conjunto, intervalo_crear(inicio, inicio));
                } else {
                  printf("Numero fuera de rango\n");
                  itree_destruir(conjunto);
                  return 1;
                }
              } else {
                  printf("Sintaxis erronea\n");
                  itree_destruir(conjunto);
                  return 1;
                }
          } else {
              printf("Numero fuera de rango\n");
              itree_destruir(conjunto);
              return 1;
            }

          tabla = hash_insertar(tabla, lista.palabras[0]->alias, conjunto);
          return 1;
        }

      } else {
          printf("Alias invalido\n");
          return 1;
        }
      }
      printf("Comando invalido\n");
      return 1;
    break;
    }
  }
}
