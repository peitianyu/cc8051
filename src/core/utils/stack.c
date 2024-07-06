#include "stack.h"

stack_t *stack_create()
{
    stack_t *stack = malloc(sizeof(stack_t));
    stack->size = 0;
    stack->top = NULL;
    return stack;
}

void stack_destroy(stack_t *stack)
{
    stack_node_t *node = stack->top;
    while (node != NULL) {
        stack_node_t *next = node->next;
        free(node);
        node = next;
    }
    free(stack);
}

void *stack_peek(stack_t *stack)
{
    return stack->top->data;
}

void *stack_pop(stack_t *stack)
{
    stack_node_t *node = stack->top;
    stack->top = node->next;
    stack->size--;
    void *data = node->data;
    free(node);
    return data;
}

void stack_push(stack_t *stack, void *data)
{
    stack_node_t *node = malloc(sizeof(stack_node_t));
    node->data = data;
    node->next = stack->top;
    stack->top = node;
    stack->size++;
}

size_t stack_size(stack_t *stack)
{
    return stack->size;
}
