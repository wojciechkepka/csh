#ifndef CSH_ENVH
#define CSH_ENVH

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

/* gets this process uid home directory.
 *
 * On error sets errno and sets value of home to NULL
 * 
 * @param home pointer to a string where value will be placed
 */
void csh_get_user_home(char *home);

/* gets this process uid username.
 *
 * On error sets errno and sets value of name to NULL
 * 
 * @param name pointer to a string where value will be placed
 */
void csh_get_username(char *name);

/* expands a path starting with `~` by allocating a new string on the heap and concating
 * current users home with the rest of the path.
 * 
 * On error returns a NULL pointer and errno is set
 * 
 * @param path a path to expand
 * @returns expanded path
 */
char *csh_expand_tilde(char *home, char *path);

#endif // CSH_ENVH
