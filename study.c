#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "dialog.h"

typedef struct {
    char *front;
    char *back;
} Card;

Card *deck = NULL;
char *deckName = NULL;
int cards;
bool backIsFront = false;

char *readUntil(FILE *input, char c);
void readDeck(FILE *input);

char *getFront(int index);
char *getBack(int index);

void multipleChoice();
void freeResponse();


int main() {

    initscr();
    keypad(stdscr, true);
    raw();
    refresh();

    srand((unsigned int)time(NULL));

    int choice;

    char menuPrompt[64] = "What would you like to do?";

    while ((choice = choose(menuPrompt, "Load deck", "Multiple choice", "Free response", NULL)) != -1) {
        if (choice == 1) {
            char *file = prompt("What is the path to your deck file?");
            FILE *input = fopen(file, "r");
            if (input != NULL) {
                readDeck(input);
                fclose(input);
            }
            else {
                char msg[64];
                sprintf(msg, "File \"%s\" not found.", file);
                say(msg);
            }
            free(file);
        }
        else if (choice == 2) {
            multipleChoice();
        }
        else if (choice == 3) {
            freeResponse();
        }
        if (deckName != NULL) {
            sprintf(menuPrompt, "[%s] What would you like to do?", deckName);
        }
    }

    endwin();

    free(deckName);
    free(deck);
}

char *getFront(int index) {
    if (backIsFront) {
        return deck[index].back;
    }
    else {
        return deck[index].front;
    }
}

char *getBack(int index) {
    if (backIsFront) {
        return deck[index].front;
    }
    else {
        return deck[index].back;
    }
}

char *readUntil(FILE *input, char c) {
    char *buf = malloc(sizeof(char));
    char tmp;
    int i = 0;
    while ((tmp = fgetc(input)) != c) {
        if (tmp == EOF) {
            free(buf);
            return NULL;
        }
        buf[i] = tmp;
        buf = realloc(buf, (i+2)*sizeof(char));
        i++;
    }
    buf[i] = '\0';
    return buf;
}

void readDeck(FILE *input) {
    if (deck != NULL) {
        free(deck);
    }
    if (deckName != NULL) {
        free(deckName);
    }

    deck = malloc(0);
    deckName = readUntil(input, '\n');

    int i;
    for (i = 0; ; i++) {
        deck = realloc(deck, (i+1)*sizeof(Card));
        deck[i].front = readUntil(input, ':');
        deck[i].back = readUntil(input, '\n');
        if (deck[i].front == NULL || deck[i].back == NULL) {
            cards = i;
            return;
        }
    }
}

void multipleChoice() {
    int score = 0;
    bool *chosenans = calloc(cards, sizeof(bool));
    int questions = (cards >= 10) ? 10 : cards;

    for (int i = 0; i < questions; i++) {
        char *answer[4];
        for (int k = 0; k < 4; k++) {
            answer[k] = NULL;
        }

        bool *chosen = calloc(cards, sizeof(bool));
        int answerplace = rand()%4;
        int answerindex;

        do {
            answerindex = rand()%cards;
        } while (chosenans[answerindex] == true);

        chosenans[answerindex] = true;
        chosen[answerindex] = true;
        
        for (int j = 0; j < 4; j++) {
            if (j != answerplace) {
                int index;
                do {
                    index = rand()%cards;
                } while (chosen[index] == true);
                chosen[index] = true;
                answer[j] = getBack(index); //deck[index].back;
            }
            else {
                answer[j] = getBack(answerindex); //deck[answerindex].back;
            }
        }

        char msg[64];
        sprintf(msg, "%d. %s", i+1, getFront(answerindex));
        int input = choose(msg, answer[0], answer[1], answer[2], answer[3]);

        if (input == answerplace+1) {
            score++;
        }
        else {
            sprintf(msg, "Sorry, the answer was \"%s.\"", answer[answerplace]);
            say(msg);
        }
        free(chosen);

    }
    char msg[32];
    sprintf(msg, "You got %d of %d correct!", score, questions);
    say(msg);
    free(chosenans);
    return;
}

void freeResponse() {
    int score = 0;
    int questions = (cards >= 10) ? 10 : cards;
    bool *answers = calloc(cards, sizeof(bool));

    for (int i = 0; i < questions; i++) {

        int index;

        do {
            index = rand()%cards;
        } while (answers[index] == true);
        answers[index] = true;

        char msg[128];
        sprintf(msg, "%d. %s", i+1, getFront(index));

        char *response = prompt(msg);

        if (response != NULL) {
            if (strcmp(response, getBack(index)) == 0) {
                score++;
            }
            else {
                do {
                    free(response);
                    sprintf(msg, "Sorry, it was \"%s.\" Please type the correct answer.", getBack(index));
                    response = prompt(msg);
                    if (response == NULL) {
                        break;
                    }
                } while (strcmp(response, getBack(index)) != 0); 
            }
            free(response);
        }
    }

    char msg[32];
    sprintf(msg, "You got %d of %d correct!", score, questions);
    say(msg);
    free(answers);
}
