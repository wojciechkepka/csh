#include "snake.h"


#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"

#define UPDATE_TIME 220
#define FOOD_SPAWN_RATE 2
#define BOARD_SIZE 40 // default board size if get_screen_size fails

static volatile int USER_INPUT;
static volatile bool READ_INPUT = true;


static void ms_sleep(const int ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

static size_t* get_screen_size()
{
  size_t* result = malloc(sizeof(size_t) * 2);
  if(!result) return NULL;

  struct winsize ws;
  int fd;

  fd = open("/dev/tty", O_RDWR);
  if(fd < 0 || ioctl(fd, TIOCGWINSZ, &ws) < 0) return NULL;

  result[0] = ws.ws_row;
  result[1] = ws.ws_col;

  close(fd);

  return result;
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
    size_t x, y;
    ChunkType ty;
    Direction direction;
    struct SnakeChunk *parent;
    struct SnakeChunk *child;
} SnakeChunk;

static SnakeChunk *new_snakechunk(const size_t x, const size_t y, const ChunkType ty)
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

static void move_snakechunk(SnakeChunk *chunk)
{
    switch (chunk->direction)
    {
        case UP: chunk->y--; break;
        case DOWN: chunk->y++; break;
        case LEFT: chunk->x--; break;
        case RIGHT: chunk->x++; break;
    }
}

static void join_chunks(SnakeChunk *head, SnakeChunk *tail)
{
    head->child = tail;
    tail->parent = head;
    tail->child = NULL;
}

static void add_snakechunk(SnakeChunk *head, SnakeChunk *add)
{
    SnakeChunk *chunk = head->child;
    do {} while ((chunk = chunk->child) != NULL);
    chunk->child = add;
    add->parent = chunk;
    add->child = NULL;
    add->direction = chunk->direction;
}

static void free_snake(SnakeChunk *head)
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
    size_t x, y;
    char **board;
} Board;

static Board *new_board(const size_t x, const size_t y)
{
    Board *b = malloc(sizeof(Board));
    if(!b) return NULL;

    b->x = x;
    b->y = y;
    b->board = malloc(y * sizeof(char *));
    if (!b->board) return NULL;

    size_t i, j;
    for(i = 0; i < y; i++)
    {
        b->board[i] = malloc((x + 1) * sizeof(char));
        if(b->board[i] == NULL) return NULL;
        for (j = 0; j < x; j++) b->board[i][j] = EMPTY;
        b->board[i][x] = '\0';
    }

    return b;
}

static void free_board(Board *b)
{
    for(size_t i = 0; i < b->y; i++)
    {
        free(b->board[i]);
    }
    free(b->board);
    free(b);
}

static bool is_border(const Board *b, const size_t x, const size_t y)
{
    return x == b->x || y == b->y || x == 0 || y == 0;
}

static void spawn_food(Board *b)
{
    size_t x, y;
    while (true)
    {
        x = random() % b->x;
        y = random() % b->y;

        if (b->board[y][x] == EMPTY)
        {
            b->board[y][x] = FOOD;
            break;
        }
    }
}

static SnakeChunk *spawn_snake(Board *b)
{
    size_t x = b->x / 2;
    size_t y = b->y / 2;

    b->board[y][x] = SNAKE_HEAD;
    return new_snakechunk(x, y, HEAD);
}

static void draw_board(const Board *b)
{
    size_t y, x;
    char border[b->x];
    String *s = empty_str(b->x * b->y);
    for (x = 0; x < b->x + 2; x++) border[x] = '-';

    append_str(s, "%s%s%s%s", ANSI_BOLD, ANSI_256_B("15"), border, ANSI_RESET_STYLE);
    append_str(s, "%s\033[%dD", ANSI_CUR_LINE_DOWN("1"), b->x + 2);
    for (y = 0; y < b->y; y++)
    {
        append_str(s, "%s%s|%s", ANSI_BOLD, ANSI_256_B("15"), ANSI_RESET_STYLE);
        for (x = 0; x < b->x; x++)
        {
            switch (b->board[y][x])
            {
                case SNAKE:
                    append_str(s, "%s%c%s", ANSI_256_F("120"), SNAKE, ANSI_RESET_STYLE);
                    break;
                case SNAKE_HEAD:
                    append_str(s, "%s%c%s", ANSI_256_F("9"), SNAKE_HEAD, ANSI_RESET_STYLE);
                    break;
                case FOOD:
                    append_str(s, "%s%c%s", ANSI_256_F("32"), FOOD, ANSI_RESET_STYLE);
                    break;

                default:
                    append_str(s, "%c", b->board[y][x]);
            }
        }
        append_str(s, "%s%s|%s", ANSI_BOLD, ANSI_256_B("15"), ANSI_RESET_STYLE);
        append_str(s, "%s\033[%dD", ANSI_CUR_LINE_DOWN("1"), b->x + 2);
    }
    append_str(s, "%s%s%s%s", ANSI_BOLD, ANSI_256_B("15"), border, ANSI_RESET_STYLE);

    printf("%s", s->buf);

    free_str(s);
}

static void *read_user_inp(void *vargp)
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

    return NULL;
}

static bool board_move_chunk(Board *b, SnakeChunk *chunk)
{
    if (is_border(b, chunk->x, chunk->y))
    {
        return false;
    }

    b->board[chunk->y][chunk->x] = EMPTY;
    move_snakechunk(chunk);            


    if (chunk->ty == HEAD) b->board[chunk->y][chunk->x] = SNAKE_HEAD;
    else if (chunk->ty == TAIL) b->board[chunk->y][chunk->x] = SNAKE;

    return true;
}

static bool board_is_ty(Board *b, size_t x, size_t y, Direction d, BoardElem ty)
{
    switch (d)
    {
        case UP: y--; break;
        case DOWN: y++; break;
        case LEFT: x--; break;
        case RIGHT: x++; break;
    }
    if (is_border(b, x, y)) return false;
    return (BoardElem)b->board[y][x] == ty;
}

int play_snake(Csh *csh, char **args)
{
    pthread_t tid;
    srand((unsigned int)time(NULL));
    size_t *screen_size = get_screen_size();
    if (screen_size == NULL)
    {
        screen_size = malloc(2 * sizeof(size_t));
        if (!screen_size) return 1;
        screen_size[0] = BOARD_SIZE;
        screen_size[1] = BOARD_SIZE;
    }
    Board *b = new_board(screen_size[1] - 3, screen_size[0] - 3);
    SnakeChunk *head = spawn_snake(b), *chunk, *last;
    head->direction = UP;
    bool grow = false, game_lost = false;
    size_t old_x, old_y;
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
    free_snake(head);
    free_board(b);
    free(screen_size);
    pthread_join(tid, NULL);
    fflush(stdout);
    fflush(stdin);

    return -1;
}
