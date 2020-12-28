#include "stack.h"

sstack_t *stack_init(size_t capacity)
{
    sstack_t *stack = malloc(sizeof(sstack_t));
    if (!stack)
    {
        return NULL;
    }

    stack->capacity = capacity;
    stack->top = -1;
    stack->items = malloc(stack->capacity * sizeof(char *));

    return stack;
}

void stack_free(sstack_t *stack)
{
    for (int i = 0; i < stack->capacity; i++)
    {
        free(stack->items[i]);
    }
    free(stack->items);
    free(stack);
}
bool stack_is_full(sstack_t *stack)
{
    return stack->top == stack->capacity - 1;
}

bool stack_is_empty(sstack_t *stack)
{
    return stack->top == -1;
}

void stack_push(sstack_t *stack, char* item)
{
    if (stack_is_full(stack)) return;

    stack->items[++stack->top] = item;
}

char *stack_pop(sstack_t *stack)
{
    if (stack_is_empty(stack)) return NULL;

    return stack->items[stack->top--];
}