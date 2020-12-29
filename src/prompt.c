#include "prompt.h"

prompt_t *csh_prompt_init(int flags, char *username, char *cwd)
{
    size_t total_size = 5; // 5 = whitespace, @, >, and \0
    prompt_t *p = malloc(sizeof(prompt_t));
    if (!p)
    {
        fprintf(stderr, "failed to allocate memory for prompt");
        return NULL;
    }
    p->flags = flags;
    if (flags & F_USER)
    {
        total_size += strlen(username);
        p->user = username;
    }
    else
    {
        p->user = NULL;
    }
    

    if (flags & F_CWD)
    {
        total_size += strlen(cwd);
        p->cwd = cwd;
    }
    else
    {
        p->cwd = NULL;
    }
    

    char *prompt = malloc(total_size * sizeof(char));
    if (!prompt)
    {
        fprintf(stderr, "failed to allocate memory for prompt");
        return NULL;
    }
    p->prompt = prompt;
    csh_prompt_update(p);

    return p;
}

void csh_prompt_update(prompt_t *p)
{
    p->prompt[0] = '\0';

    if (p->flags & F_USER)
    {
        strcat(p->prompt, p->user);
        strcat(p->prompt, "@");
    }

    if (p->flags & F_CWD)
    {
        strcat(p->prompt, p->cwd); 
    }

    if (p->flags & F_DELIM)
    {
        strcat(p->prompt, " > ");
    }
    else
    {
        strcat(p->prompt, " ");
    }
    
    p->len = strlen(p->prompt);
}

void csh_prompt_print(prompt_t *p)
{
    csh_prompt_update(p);
    write(STDOUT_FILENO, p->prompt, p->len);
}

void csh_prompt_free(prompt_t *p)
{
    free(p->prompt);
    free(p);
}