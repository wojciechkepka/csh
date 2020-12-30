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
 * On error prints the error to stdout and assigns NULL to passed in pointer.
 * 
 * @param home pointer to a string where value will be placed
 */
void csh_get_user_home(char *home);

/* gets this process uid username.
 *
 * On error prints the error to stdout and assigns NULL to passed in pointer.
 * 
 * @param home pointer to a string where value will be placed
 */
void csh_get_username(char *name);

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

#endif // CSH_ENVH