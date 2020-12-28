#include "prompt.h"

void csh_print_prompt(void)
{
    printf("%s@%s > ", USERNAME_p, CWD_p);
}