#ifndef CSH_HISTORYH
#define CSH_HISTORYH

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct
{
    char **elems;
    size_t capacity;
    size_t back;
} history_t;


history_t *history_init(size_t capacity);
bool history_is_full(history_t *hist);
int history_add(history_t *hist, char *item);
void history_free(history_t *hist);


#endif // CSH_HISTORYH