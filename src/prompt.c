#include "prompt.h"

/* calculates length of expanded format string
 *
 * @param p a pointer to a prompt
 */
static size_t prompt_calc_len(Prompt *p);

/******************************************************************************/

typedef struct
{
    char *format;
    int pos;
    size_t len;
} fmt_reader;

static fmt_reader *fread_init(char *format)
{
    fmt_reader *f = malloc(sizeof(fmt_reader));
    if (!f) return NULL;

    f->pos = -1;
    f->format = format;
    f->len = format == NULL ? 0 : strlen(f->format);

    return f;
}

static char *fread_next(fmt_reader *f)
{
    if (f->pos == -1) f->pos = 0;
    if (f->pos <= (int)(f->len - 1))
    {
        char * ret;
        ret = &f->format[f->pos];
        f->pos++;
        return ret;
    }
    return NULL;
}

static char *fread_peek(fmt_reader *f)
{
    if (f->pos <= (int)(f->len - 1))
    {
        return &f->format[f->pos];
    }
    return NULL;
}

# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
static bool fread_is_end(fmt_reader *f)
{
    return f->pos == (int)(f->len - 1);
}

static void fread_reset(fmt_reader *f)
{
    f->pos = -1;
}
# pragma GCC diagnostic pop

/******************************************************************************/

Prompt *prompt_init(char *username, char *cwd)
{
    Prompt *p = malloc(sizeof(Prompt));
    if (!p)
    {
        return NULL;
    }
    p->user = username;
    p->cwd = cwd;
    p->format = NULL;

    prompt_update_fmt(p);
    char *prompt = malloc(p->len * sizeof(char));
    if (!prompt)
    {
        return NULL;
    } 

    p->prompt = prompt;
    prompt_update(p);

    return p;
}

void prompt_set_fmt(Prompt *p, const char *format)
{
    if (p->format != NULL) free(p->format);
    p->format = malloc((strlen(format) + 1) * sizeof(char));
    if (!p->format) return;
    strcpy(p->format, format);
    p->len = prompt_calc_len(p);
}

int prompt_update_fmt(Prompt *p)
{
    char *env;
    if ((env = getenv(CSH_FORMAT_ENV)) == NULL)
    {
        prompt_set_fmt(p, CSH_DEFAULT_PROMPT);
        return 1;
    }
    else
    {
        if ((strcmp(env, p->format)) != 0)
        {
            prompt_set_fmt(p, env);
            return 1;
        }
    }

    return 0;
}

static size_t prompt_calc_len(Prompt *p)
{
    size_t len = 0;
    char *ret, *ret2;
    fmt_reader *reader = fread_init(p->format);

    while((ret = fread_next(reader)) != NULL)
    {
        switch (*ret)
        {
        case '%':
            ret2 = fread_peek(reader);
            if (!ret2) break;
            switch (*ret2)
            {
            case 'u':
                len += strlen(p->user);
                fread_next(reader);
                break;
            case 'd':
                len += strlen(p->cwd);
                fread_next(reader);
                break;
            }
            break;
        default:
            len += 1;
        }
    }
    
    free(reader);
    return len;
}

void prompt_update(Prompt *p)
{
    int changed = prompt_update_fmt(p);
    if (!changed) return;

    if (strlen(p->prompt) < p->len)
    {
        p->prompt = realloc(p->prompt, sizeof(char) * (p->len + 1));
        if (!p->prompt) return;
    }
    fmt_reader *reader = fread_init(p->format);
    p->prompt[0] = '\0';
    char *ret, *ret2;


    while((ret = fread_next(reader)) != NULL)
    {
        switch (*ret)
        {
        case '%':
            ret2 = fread_peek(reader);
            if (!ret2) return;
            switch (*ret2)
            {
            case 'u':
                strcat(p->prompt, p->user);
                fread_next(reader);
                break;
            case 'd':
                strcat(p->prompt, p->cwd);
                fread_next(reader);
                break;
            }
            break;
        default:
            strncat(p->prompt, ret, 1);
        }
    }

    free(reader);
}

void prompt_print(FILE *f, Prompt *p)
{
    prompt_update(p);
    write(fileno(f), p->prompt, p->len);
}

void prompt_free(Prompt *p)
{
    free(p->prompt);
    free(p->format);
    free(p);
}
