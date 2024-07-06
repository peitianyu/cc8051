#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>

typedef struct stack_node {
    void *data;
    struct stack_node *next;
} stack_node_t;

typedef struct stack {
    stack_node_t *top;
    size_t size;
} stack_t;

stack_t *stack_create();
void stack_destroy(stack_t *stack);
void stack_push(stack_t *stack, void *data);
void *stack_pop(stack_t *stack);
void *stack_peek(stack_t *stack);
size_t stack_size(stack_t *stack);

#endif