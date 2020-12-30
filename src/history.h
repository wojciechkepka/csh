#ifndef CSH_HISTORYH
#define CSH_HISTORYH

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


/* automatically growing data structure with ability to continiously add
 * new elements to the end
 * 
 * @field elems inner array of elements
 * @field capacity current capacity of elems
 * @field back current position in elems
 */
typedef struct
{
    char **elems;
    size_t capacity;
    size_t back;
} History;


/* initializes a history instance with specified capacity. 
 *
 * @param capacity base capacity
 * @returns a pointer to history instance or null if allocations failed
 */
History *history_init(size_t capacity);

/* checks whether maximum capacity has been reached
 *
 * @param hist a pointer to history instance
 * @returns true if hist is full and false otherwise
 */
bool history_is_full(History *hist);

/* adds a element to history by allocating space and copying its content. 
 * If number of elements exceeds capacity realloc is called to allocate
 * twice as much space.
 * 
 * @param hist a pointer to history instance
 * @param item an item to add to hist
 * @returns 0 if operation succeded, -1 if allocation or reallocation failed
 */
int history_add(History *hist, char *item);

/* frees up memory of all components of history instance
 *
 * @param hist a pointer to history instance
 */
void history_free(History *hist);


#endif // CSH_HISTORYH