#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "interfaz.h"
#include "../arbolDeIntervalos/avltree.h"
#include "hash.h"
#include "../arbolDeIntervalos/intervalo.h"


// Chequea que una string sea una cadena de numeros.
int es_un_numero (char *palabra) {
  for(int i = 0; palabra[i]!= '\0'; i++) {
    if(isdigit(palabra[i]) == 0 && !(palabra[i] == '-' && i == 0))
        return 0;
  }
  return 1;
}


// Verifica si la palabra es un numero con un caracter al final.
int es_un_numero_con_caracter(char *palabra, char caracter) {
  for (int i = 0; palabra[i]!= '\0'; i++) {
    // Numero o - de negativo
    if (isdigit(palabra[i]) == 0 && !(palabra[i] == '-' && i == 0)) {
      if (palabra[i] == caracter && palabra[i+1] == '\0')
        return 1;
      else
        return 0;
    }
  }
  return 1;
}


// Verifica que una palabra sea un alias.
int alias_validar_sintaxis (char *alias) {
  for(int i = 0; alias[i]!= '\0'; i++) {
    if(isalpha(alias[i]) == 0 && isdigit(alias[i]) == 0)
      return 0;
  }
  return 1;
}


// Retorna el conjunto asociado al alias si es que existe, sino NULL.
AVLTree alias_validar (HashTabla *tabla, char *alias) {
  return hash_buscar(tabla, alias);
}


// Chequear que el numero este en el universo a trabajar.
int long_chequear_int(long numero, char *palabra, int largoDebido) {
  if (numero > INT_MAX || numero < INT_MIN ||
      (numero == 0 && strlen(palabra) != largoDebido))
    return 0;
  return 1;
}


// Toma una palabra y crea un token asociado.
Token token_crear (char *palabra) {
  Token token;

  if (strcmp(palabra, "salir") == 0) {
    token.tipo = salir;

  } else if (strcmp(palabra, "imprimir") == 0) {
    token.tipo = imp;

  } else if (strcmp(palabra, "=") == 0) {
    token.tipo = igual;

  } else if (strcmp(palabra, "|") == 0) {
    token.tipo = unio;

  } else if (strcmp(palabra, "&") == 0) {
    token.tipo = inter;

  } else if (strcmp(palabra, "-") == 0) {
    token.tipo = resta;

  } else if (strcmp(palabra, "{x:") == 0) {
    token.tipo = corX;

  } else if (strcmp(palabra, "<=") == 0) {
    token.tipo = menorIgual;

  } else if (strcmp(palabra, "x") == 0) {
    token.tipo = x;

  } else if (strcmp(palabra, "{}") == 0) {
    token.tipo = dobleCor;

  } else if (es_un_numero(palabra)) {
    long numero = strtol(palabra, NULL, 10);
    // Chequeo de que la funcion strtol no haya dado error.
    if (!long_chequear_int(numero, palabra, 1)) {
      token.tipo = error;
    } else {
      token.tipo = num;
      // Numero esta en el rando de los ints
      token.numero = (int) numero;
    }

  } else if (es_un_numero_con_caracter(palabra, ',') == 1) {
    palabra[strlen(palabra)] = '\0';
    long numero = strtol(palabra, NULL, 10);
    // Chequeo de que la funcion strtol no haya dado error.
    if (!long_chequear_int(numero, palabra, 2)) {
      token.tipo = error;
    } else {
      token.tipo = numComa;
      token.numero = (int) numero;
    }

  } else if (es_un_numero_con_caracter(palabra, '}')) {
    palabra[strlen(palabra)] = '\0';
    long numero = strtol(palabra, NULL, 10);
    // Chequeo de que la funcion strtol no haya dado error.
    if (!long_chequear_int(numero, palabra, 2)) {
      token.tipo = error;
    } else {
      token.tipo = numCor;
      token.numero = (int) numero;
    }

  } else if (palabra[0] == '{' && es_un_numero_con_caracter(palabra + 1, ',')) {
    palabra[strlen(palabra)] = '\0';
    long numero = strtol(palabra + 1, NULL, 10);
    // Chequeo de que la funcion strtol no haya dado error.
    if (!long_chequear_int(numero, palabra, 3)) {
      token.tipo = error;
    } else {
      token.tipo = corNumComa;
      token.numero = (int) numero;
    }

  } else if (palabra[0] == '{' && es_un_numero_con_caracter(palabra + 1, '}')) {
    palabra[strlen(palabra)] = '\0';
    long numero = strtol(palabra + 1, NULL, 10);
    // Chequeo de que la funcion strtol no haya dado error.
    if (!long_chequear_int(numero, palabra, 3)) {
      token.tipo = error;
    } else {
      token.tipo = corNumCor;
      token.numero = (int) numero;
    }

  } else if (palabra[0] == '~') {
    if (alias_validar_sintaxis(palabra + 1)) {
      token.tipo = notAlias;
      token.alias = malloc(sizeof(char) * strlen(palabra));
      token.alias = strcpy(token.alias, palabra + 1);
    } else {
      token.tipo = error;
    }

  } else if (alias_validar_sintaxis(palabra)) {
    token.tipo = alias;
    token.alias = malloc(sizeof(char) * (strlen(palabra) + 1));
    token.alias = strcpy(token.alias, palabra);

  } else {
    token.tipo = error;

  }
  return token;
}


Tokens tokens_lista_crear (char *linea) {
  Tokens lista;
  // Se toma CAPACIDAD / 2 ya que la cantidad de
  // palabras puede ser a lo sumo la mitad del largo total.
  lista.palabras = calloc(CAPACIDAD / 2, sizeof(Token *));

  char *palabra = strtok(linea, " ");

  int i;
  // Se clasifica cada palabra en un token.
  for (i = 0; palabra != NULL; i++) {
    lista.palabras[i] = token_crear(palabra);
    palabra = strtok(NULL, " ");
  }

  lista.largo = i;
  return lista;
}


// Libera la lista de tokens, liberando los alias si es necesario.
void tokens_destruir(Tokens lista) {
  for(int i = 0; i < lista.largo; i++) {
    if (lista.palabras[i].tipo == alias || lista.palabras[i].tipo == notAlias)
      free(lista.palabras[i].alias);
  }
  free(lista.palabras);
}


void imprimir_conjunto(HashTabla * tabla, char *alias) {
  AVLTree conjunto = alias_validar(tabla, alias);
  if (conjunto) {
    // Se muestra el conjunto en orden
    itree_recorrer_inorder(conjunto, (Visitante)inodo_imprimir);
    puts("");
  }
  else
    printf("Conjunto no almacenado\n");
}


// Lee la lista de tokens e inserta si es posible el intervalo definido.
void insertar_conjunto_compresion (HashTabla *tabla, Tokens lista) {
  if (lista.largo != 8) {
    printf("Comando invalido \n");
    return;
  }

  // Chequeo de completitud de la sintxis.
  if (lista.palabras[3].tipo == num && lista.palabras[4].tipo == menorIgual && lista.palabras[5].tipo == x && lista.palabras[6].tipo == menorIgual && lista.palabras[7].tipo == numCor) {
    Intervalo intervalo = intervalo_crear(lista.palabras[3].numero, lista.palabras[7].numero);
    if (intervalo_validar(intervalo)) {
      // Si el intervalo es valido
      hash_insertar(tabla, lista.palabras[0].alias, itree_insertar(NULL, intervalo));
      return;
    }
    // Si el inicio es mayor al final, el conjunto es el vacio.
    hash_insertar(tabla, lista.palabras[0].alias, itree_insertar(NULL, VACIO));
    return;
  }
  printf("Comando invalido \n");
}


// Lee la lista de tokens e inserta si es posible el intervalo definido.
void insertar_conjunto_extension (HashTabla *tabla, Tokens lista) {
  // Verificaciones que nos ahorran trabajo.
  if (lista.largo < 4) {
    printf("Comando invalido\n");
    return;
  }

  if (lista.palabras[2].tipo != corNumComa) {
    printf("Comando invalido\n");
    return;
  }

  AVLTree conjunto = itree_crear();

  // Si la sitaxis es correcta se agregan los intervalos al arbol resultado.
  int numero = lista.palabras[2].numero;
  conjunto = itree_insertar(conjunto, intervalo_crear(numero, numero));

  int i = 3;

  for (; lista.palabras[i].tipo == numComa; i++) {
    numero = lista.palabras[i].numero;
    conjunto = itree_insertar_disjutos(conjunto, intervalo_crear(numero, numero));
  }

  // Si es correcto se alamacena en la tabla hash.
  if (lista.palabras[i].tipo == numCor) {
    numero = lista.palabras[i].numero;
    conjunto = itree_insertar_disjutos(conjunto, intervalo_crear(numero, numero));
    hash_insertar(tabla, lista.palabras[0].alias, conjunto);
    return;
  }
  // Sino se libera y se notifica el error.
  printf("Comando invalido\n");
  itree_destruir(conjunto);

}


void insertar_conjunto_un_elem (HashTabla *tabla, char *alias, int numero) {
  Intervalo intervalo = intervalo_crear(numero, numero);
  if (intervalo_validar(intervalo))
    // Se chequea que el numero este dentro el rando -INFINITO, INFINITO.
    hash_insertar(tabla, alias, itree_insertar(NULL, intervalo));
  else
    printf("Elmento invalido\n");
}


void insertar_conjunto_vacio(HashTabla *tabla, char *alias) {
  hash_insertar(tabla, alias, itree_insertar(NULL, VACIO));
}


// Lee el comando y realiza el complemento.
void insertar_complemento(HashTabla *tabla, char *aliasAlmacenar, char *alias) {
  AVLTree operando = alias_validar(tabla, alias);
  // Si operando es NULL, el alias no esta almacenado.
  if (operando)
     hash_insertar(tabla, aliasAlmacenar, itree_complemento(operando));
  else
    printf("Alias no almacenado\n");
}


void insertar_operacion(HashTabla *tabla, Tokens lista) {
  // Verificaciones que ahorran computo.
  if (lista.palabras[2].tipo != alias || lista.palabras[4].tipo != alias) {
    printf("Sintaxis invalida\n");
    return;
  }
  if (lista.palabras[3].tipo != unio && lista.palabras[3].tipo != resta && lista.palabras[3].tipo != inter) {
    printf("Operacion invalida\n");
    return;
  }
  AVLTree operando1 = alias_validar(tabla, lista.palabras[2].alias);
  AVLTree operando2 = alias_validar(tabla, lista.palabras[4].alias);
  // Se chequea que los alias funcionen.
  if (operando1 && operando2) {
    switch (lista.palabras[3].tipo) {
      case resta: {
        hash_insertar(tabla, lista.palabras[0].alias, itree_resta(operando1, operando2));
        break;
      }
      case inter: {
        hash_insertar(tabla, lista.palabras[0].alias, itree_interseccion(operando1, operando2));
        break;
      }
      case unio: {
        hash_insertar(tabla, lista.palabras[0].alias, itree_union(operando1, operando2));
        break;
      }
      default: {
        // Caso operador desconocido.
        printf("Operacion invalida\n");
        break;
      }
    }
  } else {
     printf("Alias invalido\n");
  }
}


int ejecutar_comando(HashTabla * tabla, Tokens lista) {
  // Switch sobre el tipo de la primir palabra.
  // Segun el tipo se ve que operacion se debe realizar.
  switch (lista.palabras[0].tipo) {

    case salir: {
      if (lista.largo == 1) {
        printf("Saliendo\n");
        return 0;
      }
      break;
    }

    case imp: {
      if (lista.largo == 2) {
        imprimir_conjunto(tabla, lista.palabras[1].alias);
        return 1;
      }
      break;
    }

    case alias: {

      if (lista.largo < 3 || lista.palabras[1].tipo != igual) {
        printf("Comando invalido\n");
        return 1;
      }
      // Switch sobre el tipo de la tercer palabra.
      // Dependiendo de este tipo, ejecutamos distintas acciones.
      switch (lista.palabras[2].tipo) {
        case corX: {
          insertar_conjunto_compresion(tabla, lista);
          return 1;
        }

        case corNumComa: {
          insertar_conjunto_extension(tabla, lista);
          return 1;
        }

        case corNumCor: {
          if (lista.largo == 3) {
            insertar_conjunto_un_elem(tabla, lista.palabras[0].alias, lista.palabras[2].numero);
            return 1;
          }
          break;
        }

        case dobleCor: {
          if (lista.largo == 3) {
            insertar_conjunto_vacio(tabla, lista.palabras[0].alias);
            return 1;
          }
          break;
        }

        case notAlias: {
          if (lista.largo == 3) {
            insertar_complemento(tabla, lista.palabras[0].alias, lista.palabras[2].alias);
            return 1;
          }
          break;
        }

        case alias: {
          if (lista.largo == 5) {
            insertar_operacion(tabla, lista);
            return 1;
          }
        }

        default:
          break;
      }
    }

    default:
      break;
  }

  printf("Comando invalido\n");
  return 1;
}
