#ifndef CSH_CMDH
#define CSH_CMDH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "path.h"
#include "env.h"
#include "csh.h"


/******************************************************************************/

/* number of builtin commands calculated from builtin_funcs array
 */
#define CSH_BUILTIN_COUNT (sizeof(builtin_funcs) / sizeof(int (*)(csh_t *, char **)))

/* internal implementation of cd builtin command.
 *
 * @param path a path to change directory to.
 */
void _csh_cd(csh_t *csh, char *path);

/* builtin implementation of cd command.
 *
 * @param args arguments to call cd with.
 * @returns always 1 to indicate another loop
 */
int csh_cd(csh_t *csh, char **args);

/* builtin implementation of help command.
 *
 * @param args arguments to call help with.
 * @returns always 1 to indicate another loop
 */
int csh_help(csh_t *csh, char **args);

/* builtin implementation of exit command.
 *
 * @param args arguments to call exit with.
 * @returns always 0 to indicate end of the loop
 */
int csh_exit(csh_t *csh, char **args);

/* bultin command ~. It displays users home directory and changes CWD to it.
 *
 * @param args arguments to call ~ with
 * @returns always 1 to indicate another loop
 */
int csh_tilde(csh_t *csh, char **args);

/* builtin implementation of export command. This command is used to set
 * environment variables in current shell for other processes to use
 * 
 * @param args arguments to call export with
 * @returns always 1 to indicate another loop
 */
int csh_export(csh_t *csh, char **args);

/* builtin implementation of unset command. This command is used to unset
 * an environment variable in current shell.
 * 
 * @param args arguments to call unset with
 * @returns always 1 to indicate another loop
 */
int csh_unset(csh_t *csh, char **args);

/* builtin implementation of history command. This command is used to display
 * history of executed commands.
 * 
 * @param args arguments to call history with
 * @returns always 1 to indicate another loop
 */
int csh_history(csh_t *csh, char **args);

/******************************************************************************/

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
int csh_execute(csh_t *csh, char **args);


/******************************************************************************/


/* a list of builtin command names
 */
extern const char *builtin_commands[];

/* a list of pointers to builtin functions
 */
extern int (*builtin_funcs[]) (csh_t *, char **);

#endif // CSH_CMDH