#ifndef CSH_STACKH
#define CSH_STACKH

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct
{
    char **items;
    ssize_t top;
    size_t capacity;
} sstack_t;

sstack_t *stack_init(size_t capacity);
void stack_free(sstack_t *stack);
bool stack_is_full(sstack_t *stack);
bool stack_is_empty(sstack_t *stack);
void stack_push(sstack_t *stack, char* item);
char *stack_pop(sstack_t *stack);


#endif // CSH_STACKH