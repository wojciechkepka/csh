#ifndef CSH_PATHH
#define CSH_PATHH

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* expands a path starting with `~` by allocating a new string on the heap and concating
 * current users home with the rest of the path.
 * 
 * If this function fails to allocate space for new string a NULL pointer is returned
 * and an error message is printed to stdout.
 * 
 * @param path a path to expand
 * @returns expanded path
 */
char *csh_expand_tilde(char *home, char *path);

#endif // CSH_PATHH