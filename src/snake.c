#include "snake.h"

#define UPDATE_TIME 220
#define FOOD_SPAWN_RATE 2
#define BOARD_SIZE 40

static volatile int USER_INPUT;
static volatile bool READ_INPUT = true;


void ms_sleep(int ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

typedef enum BoardElem
{
    EMPTY = ' ',
    FOOD = '*',
    SNAKE_HEAD = '@',
    SNAKE = '#',
} BoardElem;

typedef enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
} Direction;

typedef enum ChunkType
{
    HEAD,
    TAIL,
} ChunkType;

typedef struct SnakeChunk
{
    int x, y;
    ChunkType ty;
    Direction direction;
    struct SnakeChunk *parent;
    struct SnakeChunk *child;
} SnakeChunk;

SnakeChunk *new_snakechunk(int x, int y, ChunkType ty)
{
    SnakeChunk *s = malloc(sizeof(SnakeChunk));
    if (!s) return NULL;

    s->x = x;
    s->y = y;
    s->ty = ty;
    s->direction = RIGHT;
    s->parent = NULL;
    s->child = NULL;

    return s;

}

void move_snakechunk(SnakeChunk *chunk)
{
    switch (chunk->direction)
    {
        case UP: chunk->y--; break;
        case DOWN: chunk->y++; break;
        case LEFT: chunk->x--; break;
        case RIGHT: chunk->x++; break;
    }
}

void join_chunks(SnakeChunk *head, SnakeChunk *tail)
{
    head->child = tail;
    tail->parent = head;
    tail->child = NULL;
}

void add_snakechunk(SnakeChunk *head, SnakeChunk *add)
{
    SnakeChunk *chunk = head->child;
    do {} while ((chunk = chunk->child) != NULL);
    chunk->child = add;
    add->parent = chunk;
    add->child = NULL;
    add->direction = chunk->direction;
}

void free_snake(SnakeChunk *head)
{
    SnakeChunk *chunk = head->child;
    do
    {
        free(chunk);
    }
    while ((chunk = chunk->child) != NULL);

    free(head);
}

typedef struct Board
{
    int x;
    char **board;
} Board;

Board *new_board(int x)
{
    Board *b = malloc(sizeof(Board));
    if(!b) return NULL;

    b->x = x;
    b->board = malloc(x * sizeof(char *));
    if (!b->board) return NULL;

    int i, j;
    for(i = 0; i < x; i++)
    {
        b->board[i] = malloc((x + 1) * sizeof(char));
        if(b->board[i] == NULL) return NULL;
        for (j = 0; j < x; j++) b->board[i][j] = EMPTY;
        b->board[i][x] = '\0';
    }

    return b;
}

void free_board(Board *b)
{
    for(int i = 0; i < b->x; i++)
    {
        free(b->board[i]);
    }
    free(b->board);
    free(b);
}

bool is_border(Board *b, int x, int y)
{
    return x == b->x || y == b->x || x == -1 || y == -1;
}

void spawn_food(Board *b)
{
    int x, y;
    while (true)
    {
        x = rand() % b->x;
        y = rand() % b->x;

        if (b->board[y][x] == EMPTY)
        {
            b->board[y][x] = FOOD;
            break;
        }
    }
}

SnakeChunk *spawn_snake(Board *b)
{
    int x = b->x / 2;
    int y = b->x / 2;

    b->board[y][x] = SNAKE_HEAD;
    return new_snakechunk(x, y, HEAD);
}

void draw_board(Board *b)
{
    int i;
    char border[b->x];
    for (i = 0; i < b->x + 2; i++) border[i] = '-';

    printf("%s", border);
    printf("%s\033[%dD", ANSI_CUR_LINE_DOWN("1"), b->x + 2);
    for (int i = 0; i < b->x; i++)
    {
        printf("|%s|", b->board[i]);
        printf("%s\033[%dD", ANSI_CUR_LINE_DOWN("1"), b->x + 2);
    }
    printf("%s", border);
}

void *read_user_inp(void *vargp)
{
    int key;
    while (READ_INPUT)
    {
        key = csh_readkey();
        if (key == ARROW_UP || key == ARROW_DOWN || key == ARROW_LEFT || key == ARROW_RIGHT)
        {
            USER_INPUT = key;
        }
    }
}

bool board_move_chunk(Board *b, SnakeChunk *chunk)
{

    b->board[chunk->y][chunk->x] = EMPTY;
    move_snakechunk(chunk);            
    if (is_border(b, chunk->x, chunk->y))
    {
        return false;
    }


    if (chunk->ty == HEAD) b->board[chunk->y][chunk->x] = SNAKE_HEAD;
    else if (chunk->ty == TAIL) b->board[chunk->y][chunk->x] = SNAKE;

    return true;
}

bool board_is_ty(Board *b, int x, int y, Direction d, BoardElem ty)
{
    switch (d)
    {
        case UP: y--; break;
        case DOWN: y++; break;
        case LEFT: x--; break;
        case RIGHT: x++; break;
    }
    if (is_border(b, x, y)) return false;
    return b->board[y][x] == ty;
}

int play_snake(Csh *csh, char **args)
{
    pthread_t tid;
    srand(time(NULL));
    Board *b = new_board(BOARD_SIZE);
    SnakeChunk *head = spawn_snake(b), *chunk, *last;
    head->direction = UP;
    char choice;
    bool grow = false, game_lost = false;
    int old_x, old_y;
    Direction old_dir;

    pthread_create(&tid, NULL, read_user_inp, NULL);
    csh_enable_raw_mode();
    for (int i = 0;;i++)
    {
        printf("%s%s", ANSI_CUR_HOME, ANSI_CLS);
        if (i % FOOD_SPAWN_RATE == 0) spawn_food(b);
        draw_board(b);
        fflush(stdout);

        switch (USER_INPUT)
        {
            case ARROW_UP: if (head->direction != DOWN) head->direction = UP; break;
            case ARROW_DOWN: if (head->direction != UP) head->direction = DOWN; break;
            case ARROW_LEFT: if (head->direction != RIGHT) head->direction = LEFT; break;
            case ARROW_RIGHT: if (head->direction != LEFT) head->direction = RIGHT; break;
        };

        chunk = head;
        
        while (true)
        {
            if (chunk->ty == HEAD)
            {
                if (board_is_ty(b, chunk->x, chunk->y, chunk->direction, FOOD)) grow = true;
                if (board_is_ty(b, chunk->x, chunk->y, chunk->direction, SNAKE))
                {
                    game_lost = true;
                    break;
                }
            }

            if (chunk->child == NULL)
            {
                last = chunk;
                old_x = last->x;
                old_y = last->y;
                old_dir = last->direction;
                
                if (!board_move_chunk(b, chunk))
                {
                    game_lost = true;
                    break;
                }

                do
                {
                    if (chunk->ty == TAIL)
                    {
                        if (chunk->parent->direction != chunk->direction) chunk->direction = chunk->parent->direction;
                    }
                }
                while ((chunk = chunk->parent) != NULL);
                break;
            }

            board_move_chunk(b, chunk);

            chunk = chunk->child;
        }

        if (game_lost)
        {
            READ_INPUT = false;
            break;
        }

        if (grow)
        {
            grow = false;
            SnakeChunk *s = new_snakechunk(old_x, old_y, TAIL);
            s->direction = old_dir;
            b->board[s->y][s->x] = SNAKE;
            join_chunks(last, s);
            continue;
        }
        
        ms_sleep(UPDATE_TIME);
    }
    printf("%s%s", ANSI_CUR_HOME, ANSI_CLS);
    csh_disable_raw_mode();
    printf("Game over!\n");
    free_board(b);
    pthread_join(tid, NULL);
    fflush(stdout);
    fflush(stdin);
}
