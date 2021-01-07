#ifndef CSH_ANSIH
#define CSH_ANSIH

// cursor

#define ANSI_CUR_HOME "\033[H" // moves cursor to position 0,0
#define ANSI_CUR_XY(line, column) ("\033[" line ";" column "H")
#define ANSI_CUR_LINE_UP(n) ("\033[" n "A")
#define ANSI_CUR_LINE_DOWN(n) ("\033[" n "B")
#define ANSI_CUR_COL_RIGHT(n) ("\033[" n "C")
#define ANSI_CUR_COL_LEFT(n) ("\033[" n "D")
#define ANSI_CUR_COL(n) ("\033[" n "G")
#define ANSI_CUR_SAVE "\033[s"
#define ANSI_CUR_RESTORE "\033[u"

// erase

#define ANSI_CLEAR_EOS "\033[0J" // from cursor to end of screen
#define ANSI_CLEAR_BOS "\033[1J" // from cursor to beggining of screen
#define ANSI_CLS "\033[2J" // clears entire screen
#define ANSI_CLL_EOL "\033[0K" // from cursor to end of line
#define ANSI_CLL_BOL "\033[1K" // from cursor to beggining of line
#define ANSI_CLL "\033[2K" // clears entire line

// styles

#define ANSI_RESET_STYLE "\033[0m"
#define ANSI_BOLD "\033[1m"
#define ANSI_DIM "\033[2m"

// colors

#define ANSI_RGB_F(r, g, b) ("\033[38;2;" r ";" g ";" b ";m")
#define ANSI_RGB_B(r, g, b) ("\033[48;2;" r ";" g ";" b ";m")
#define ANSI_256_F(color_id) ("\033[38;5;" color_id "m")
#define ANSI_256_B(color_id) ("\033[48;5;" color_id "m")



#endif // CSH_ANSIH
