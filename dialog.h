//Quick library for displaying things on screen w/ ncurses


//Returns # of choice. Pass amount of choices and a list of char*
int choose(char *message, int optc, ...);

//Prompts for a string. Make sure to free() returned string
char *prompt(char *message);

//Displays message and waits for user input
void say(char *message);

//Displays a message for sec seconds
void show(char *message, double sec);


//shhhhhh
void resetChoice();
