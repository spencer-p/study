//Quick library for displaying things on screen w/ ncurses


//Returns # of choice. Pass NULL for no option
int choose(char *message, char *opt1, char *opt2, char *opt3, char *opt4);

//Prompts for a string. Returns MALLOCED string
char *prompt(char *message);

void say(char *message);
