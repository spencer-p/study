#include "dialog.h"
#include <ncurses.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

typedef struct {
    int x, y;
} point;

short dialogChoice = 1;
 
WINDOW *makeWindow(void) {
    WINDOW *window = newwin(LINES-2, COLS-2, 1, 1);
    keypad(window, true);
    return window;
}

int charcount(char *str, char c) {
    char count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        count += (str[i] == c) ? 1 : 0;
    }
    return count;
}

void resetChoice() {
    dialogChoice = 1;
}

int choose(char *message, int optc, ...) {

    WINDOW *window = makeWindow();
    noecho();
    curs_set(0);

    mvwprintw(window, 2, 2, "%s", message);

    va_list args;
    va_start(args, optc);
    char **options = malloc(optc*sizeof(char*));
    for (int i = 0; i < optc; i++) {
        options[i] = va_arg(args, char*);
        mvwprintw(window, 4+(2*i), 4, "%s", options[i]);
    }
    va_end(args);

    if (dialogChoice > optc) {
        dialogChoice = optc;
    }
    
    mvwprintw(window, 4+(2*(dialogChoice-1)), 2, ">");
    box(window, 0, 0);
    wrefresh(window);

    int c, prev = dialogChoice;

    while ((c=getch()) != '\n') {
        switch (c) {
            case KEY_DOWN:
            case 'j':
            case 'J':
                dialogChoice += (dialogChoice == optc) ? 0 : 1;
                break;
            case KEY_UP:
            case 'k':
            case 'K':
                dialogChoice -= (dialogChoice == 1) ? 0 : 1;
                break;
            case 27: //escape key
            case 'q':
            case 'Q':
                wrefresh(window);
                delwin(window);
                return -1;
                break;
        }
        
        mvwprintw(window, 4+(2*(prev-1)), 2, " ");
        prev = dialogChoice;
        mvwprintw(window, 4+(2*(dialogChoice-1)), 2, ">");
        wrefresh(window);
    }

    wrefresh(window);
    delwin(window);

    return dialogChoice;
}

char *prompt(char *message) {

    WINDOW *window = makeWindow();
    noecho();
    curs_set(1);

    int lines = charcount(message, '\n');

    mvwprintw(window, 2, 2, "%s", message);
    box(window, 0, 0);
    wmove(window, 4+lines, 4);

    wrefresh(window);

    int c, i = 0;
    char *answer = malloc(sizeof(char));
    answer[i] = '\0';
    
    while ((c = getch()) != '\n') {
        switch (c) {
            case 127: //delete key
            case KEY_BACKSPACE:
            case KEY_DC:
                if (i != 0) {
                    i--;
                    answer = realloc(answer, (i+1)*sizeof(char));
                    answer[i] = '\0';
                    mvwaddch(window, 4+lines, 4+i, ' ');
                }
                break;
            case 27: //escape key
                wrefresh(window);
                delwin(window);
                free(answer);
                return NULL;
            default:
                answer[i] = c;
                i++;
                answer = realloc(answer, (i+1)*sizeof(char));
                answer[i] = '\0';
                break;
        }
        mvwprintw(window, 4+lines, 4, "%s", answer);
        wrefresh(window);
    }

    wrefresh(window);
    delwin(window);
    return answer;
}

void say(char *message) {

    WINDOW *window = makeWindow();
    noecho();
    curs_set(0);

    int lines = charcount(message, '\n');
    
    mvwprintw(window, 2, 2, "%s", message);
    mvwprintw(window, 4+lines, 2, "[Enter]");
    box(window, 0, 0);

    wrefresh(window);
    
    char c;
    while ((c = getch()) != '\n') {
        //do nothing
    }

    wrefresh(window);
    delwin(window);
    return;
}

void show(char *message, double sec) {

    WINDOW *window = makeWindow();
    noecho();
    curs_set(0);

    mvwprintw(window, 2, 2, "%s", message);
    box(window, 0, 0);

    wrefresh(window);
    
    usleep(sec*1000000);

    wrefresh(window);
    delwin(window);
    return;
}
