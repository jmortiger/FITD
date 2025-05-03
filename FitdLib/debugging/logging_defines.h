#ifndef __H_LOGGING_DEFINES__
#ifndef ANSI_RESET
#define ANSI_RESET "\033[0m"
#define ANSI_BLACK 0
#define ANSI_RED 1
#define ANSI_GREEN 2
#define ANSI_YELLOW 3
#define ANSI_BLUE 4
#define ANSI_MAGENTA 5
#define ANSI_CYAN 6
#define ANSI_WHITE 7
#endif
#define ANSI_FG_DARK 3
#define ANSI_BG_DARK 4
#define ANSI_FG_LIGHT 9
#define ANSI_BG_LIGHT 10
#define ANSI_FG_FULL 3 //8;5;
#define ANSI_BG_FULL 4 //8;5;
#define ANSI_BLINK "\033[98;5m"
#define _TermFullColor(fgBg, color) "\033[" #fgBg "8;5;" #color "m"
#define TermFullColor(fgBg, color) _TermFullColor(fgBg, color)
#define _TermCol(fgBg, color) "\033[" #fgBg #color "m"
#define TermCol(fgBg, color) _TermCol(fgBg, color)
#define _TermCol2(fgBg1, color1, fgBg2, color2) "\033[" #fgBg1 #color1 ";" #fgBg2 #color2 "m"
#define TermCol2(fgBg1, color1, fgBg2, color2) _TermCol2(fgBg1, color1, fgBg2, color2)
#define ANSI_FG_DARK_GREY _TermFullColor(3, 241)
// TermCol(ANSI_FG_LIGHT, ANSI_WHITE)
// TermCol2(ANSI_FG_LIGHT, ANSI_WHITE, ANSI_BG_DARK, ANSI_RED)
#endif