#ifndef INTERVALO_H_INCLUDED
#define INTERVALO_H_INCLUDED
#include <limits.h>

// Menor mayor int aceptado en general. Solo por comodidad de notacion.
#define INFINITO INT_MAX
// Representacion del intervalo vacio.
#define VACIO ((Intervalo){.inicio = 1, .final = -1})

typedef struct _Intervalo {
    int inicio;
    int final;
}   Intervalo;

// max toma dos int y devuelve el mayor.
int max(int a, int b);

// min toma dos int y devuelve el menor.
int min(int a, int b);

// intervalo_crear toma dos ints y crea una estructura intervalo.
Intervalo intervalo_crear(int, int);

// intervalo_validar valida si el intervalo es correcto.
int intervalo_validar(Intervalo);

// intervalo_imprimir imprime en pantalla el intervalo.
void intervalo_imprimir(Intervalo);

// intervalo_copiar retorna una estructura Intervalo igual a la pasada.
Intervalo intervalo_copiar(Intervalo);

// intervalo_interseccion determina si dos intervalos tienen interseccion.
int intervalo_interseccion(Intervalo, Intervalo);

// intervalo_valor_interseccion retorna el intervalo interseccion entre dos.
Intervalo intervalo_valor_interseccion(Intervalo, Intervalo);

// intervalo_aumentado retonra un intervalo aumentado en 1 unidad de ser posible
Intervalo intervalo_aumentado(Intervalo);

#endif
