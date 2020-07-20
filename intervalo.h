#ifndef INTERVALO_H_INCLUDED
#define INTERVALO_H_INCLUDED
#define INFINITO 32768
#define VACIO ((Intervalo){.inicio = 1, .final = -1})

typedef struct _Intervalo {
    int inicio;
    int final;
}Intervalo;

void intervalo_imprimir(Intervalo);
Intervalo intervalo_copiar(Intervalo);
int intervalo_interseccion(Intervalo, Intervalo);
Intervalo intervalo_crear(int, int);
int intervalo_validar(Intervalo);
#endif // INTERVALO_H_INCLUDED
