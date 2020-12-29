#include "history.h"

history_t *history_init(size_t capacity)
{
    history_t *hist = malloc(sizeof(history_t));
    char **elems = malloc(capacity * sizeof(char *));
    if (!elems)
    {
        return NULL;
    }

    hist->elems = elems;
    hist->capacity = capacity;
    hist->back = 0;

    return hist;
}

bool history_is_full(history_t *hist)
{
    return hist->back == (hist->capacity - 1);
}

int history_add(history_t *hist, char *item)
{
    if (hist->back > hist->capacity - 1) hist->back = 0;
    if (history_is_full(hist))
    {
        hist->elems = realloc(hist->elems, hist->capacity * 2);
        if (!hist->elems) return -1;
        hist->capacity *= 2;
    }

    char *add = malloc((strlen(item)+1) * sizeof(char));
    if (!add) return -1;

    strcpy(add, item);
    hist->elems[hist->back] = add; 
    hist->back++;

    return 0;
}

void history_free(history_t *hist)
{
    for (size_t i = 0; i < hist->back; i++)
    {
        free(hist->elems[i]);
    }
    free(hist->elems);
    free(hist);
}