#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "dlist/dlist.h"

struct _Stack{
  DList primero;
};

Stack stack_new() {
    Stack stack = malloc(sizeof(struct _Stack));
    stack->primero = dlist_crear();
    return stack;
}

int stack_isEmpty(Stack stack) {
  return stack->primero == NULL;
}

void stack_push(Stack stack, void *dato) {
  stack->primero = dnodo_agregar_inicio(stack->primero, dato);
}

void stack_pop(Stack stack) {
  stack->primero = dlist_eliminar_primero(stack->primero);
}

void *stack_top(Stack stack) {
  if (stack_isEmpty(stack))
    return NULL;
  return dnodo_dato(stack->primero);
}

void stack_destruir(Stack stack) {
  dlist_destruir(stack->primero, NULL);
  free(stack);
}
