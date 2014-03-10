#include "dialog.h"
#include <ncurses.h>
#include <stdlib.h>

typedef struct {
    int x, y;
} point;

WINDOW *drawBox(void) {
    WINDOW *window = newwin(LINES-2, COLS-2, 1, 1);
    box(window, 0, 0);
    keypad(window, true);
    return window;
}

int choose(char *message, char *opt1, char *opt2, char *opt3, char *opt4) {

    WINDOW *window = drawBox();
    noecho();
    curs_set(0);
    
    mvwprintw(window, 2, 2, "%s", message);

    mvwprintw(window, 4, 4, "%s", opt1);
    mvwprintw(window, 6, 4, "%s", opt2);
    mvwprintw(window, 8, 4, "%s", (opt3 != NULL) ? opt3 : "");
    mvwprintw(window, 10, 4, "%s", (opt4 != NULL) ? opt4 : "");

    mvwprintw(window, 4, 2, ">");
    wrefresh(window);

    int c, choice = 1, prev = 1;

    while ((c=getch()) != '\n') {
        switch (c) {
            case KEY_DOWN:
            case 'j':
            case 'J':
                choice += (choice == 4) ? 0 : 1;
                break;
            case KEY_UP:
            case 'k':
            case 'K':
                choice -= (choice == 1) ? 0 : 1;
                break;
            case '1':
                choice = 1;
                break;
            case '2':
                choice = 2;
                break;
            case '3':
                choice = 3;
                break;
            case '4':
                choice = 4;
                break;
            case 27: //escape key
            case 'q':
                wclear(window);
                wrefresh(window);
                delwin(window);
                return -1;
                break;
        }
        
        if ((choice == 3 && opt3 == NULL) || (choice == 4 && opt4 == NULL)) {
            choice = prev;
        }

        mvwprintw(window, 4+(2*(prev-1)), 2, " ");
        prev = choice;
        mvwprintw(window, 4+(2*(choice-1)), 2, ">");
        wrefresh(window);
    }

    wclear(window);
    wrefresh(window);
    delwin(window);

    return choice;
}

char *prompt(char *message) {

    WINDOW *window = drawBox();
    noecho();
    curs_set(1);

    mvwprintw(window, 2, 2, "%s", message);
    wmove(window, 4, 4);

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
                    mvwaddch(window, 4, 4+i, ' ');
                }
                break;
            case 27: //escape key
                wclear(window);
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
        mvwprintw(window, 4, 4, "%s", answer);
        wrefresh(window);
    }

    wclear(window);
    wrefresh(window);
    delwin(window);
    return answer;
}

void say(char *message) {

    WINDOW *window = drawBox();
    noecho();
    curs_set(0);
    
    mvwprintw(window, 2, 2, "%s", message);
    mvwprintw(window, 4, 2, "[Enter]");

    wrefresh(window);
    
    char c;
    while ((c = getch()) != '\n') {
        //do nothing
    }

    wclear(window);
    wrefresh(window);
    delwin(window);
    return;
}
