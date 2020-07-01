#ifndef CONJUNTOS_H_INCLUDED
#define CONJUNTOS_H_INCLUDED

typedef struct _ConjExtension{
  int cantidad;
  int *elementos;
} ConjExtension;

typedef struct _ConjCompresion{
  int inicio;
  int final;
} ConjCompresion;


typedef struct _Conjunto{
  int tipo;
  ConjExtension *conj_extension;
  ConjCompresion *conj_compresion;
} Conjunto;



Conjunto crear_conjunto(int, int *, int);

void liberar_conjunto(Conjunto);

void mostrar_conjunto(Conjunto);

#endif
