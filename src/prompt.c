#include "prompt.h"

void csh_prompt_print(void)
{
    csh_prompt_update();
    printf("%s", PROMPT->prompt);
}

void csh_prompt_init(int flags)
{
    size_t total_size = 5; // 5 = whitespace, @, >, and \0
    PROMPT = malloc(sizeof(prompt_t));
    if (!PROMPT)
    {
        fprintf(stderr, "failed to allocate memory for prompt");
        return;
    }
    PROMPT->flags = flags;
    if (flags & F_USER)
    {
        total_size += strlen(USERNAME_p);
        PROMPT->user = USERNAME_p;
    }
    else
    {
        PROMPT->user = NULL;
    }
    

    if (flags & F_CWD)
    {
        total_size += strlen(CWD_p);
        PROMPT->cwd = CWD_p;
    }
    else
    {
        PROMPT->cwd = NULL;
    }
    

    char *prompt = malloc(total_size * sizeof(char));
    if (!prompt)
    {
        fprintf(stderr, "failed to allocate memory for prompt");
        return;
    }
    PROMPT->prompt = prompt;
    csh_prompt_update();
}

void csh_prompt_update(void)
{
    PROMPT->prompt[0] = '\0';

    if (PROMPT->flags & F_USER)
    {
        strcat(PROMPT->prompt, PROMPT->user);
        strcat(PROMPT->prompt, "@");
    }

    if (PROMPT->flags & F_CWD)
    {
        strcat(PROMPT->prompt, PROMPT->cwd); 
    }

    if (PROMPT->flags & F_DELIM)
    {
        strcat(PROMPT->prompt, " > ");
    }
    else
    {
        strcat(PROMPT->prompt, " ");
    }
    
}

void csh_prompt_free(void)
{
    free(PROMPT->prompt);
    free(PROMPT);
}