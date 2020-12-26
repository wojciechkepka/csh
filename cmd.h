#ifndef CSH_CMDH
#define CSH_CMDH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CSH_BUILTIN_COUNT 4

extern char *USERHOME_p, *USERNAME_p;

/* internal implementation of cd builtin command.
 *
 * @param path a path to change directory to.
 */
void _csh_cd(char *path);

/* builtin implementation of cd command.
 *
 * @param args arguments to call cd with.
 * @returns always 1 to indicate another loop
 */
int csh_cd(char **args);

/* builtin implementation of help command.
 *
 * @param args arguments to call help with.
 * @returns always 1 to indicate another loop
 */
int csh_help(char **args);

/* builtin implementation of exit command.
 *
 * @param args arguments to call exit with.
 * @returns always 0 to indicate end of the loop
 */
int csh_exit(char **args);

/* bultin command ~. It displays users home directory and changes CWD to it.
 *
 * @param args arguments to call ~ with
 * @returns always 1 to indicate another loop
 */
int csh_tilde(char **args);

/**/

/* launches specified command command with args as child process.
 *
 * @param args - an array with executable name as first element and arguments as rest
 * @returns always 1 to indicate another loop
 */
int csh_launch(char **args);

/* executes arguments parsed from a line of input. This function first searches for matches
 * in builtin commands. If no match is found args are executed as a command.
 *
 * @param args - command with arguments to run
 */
int csh_execute(char **args);

/* a list of builtin command names
 */
extern const char *builtin_commands[];

/* a list of pointers to builtin functions
 */
extern int (*builtin_funcs[]) (char **);

#endif // CSH_CMDH