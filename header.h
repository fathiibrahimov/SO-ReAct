#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define SCREENHEIGHT 30
#define SCREENWIDTH 100
#define START_Y 2
#define START_X 4

#define MAXWORDLENGTH 23
#define WORDSCOUNT 6394

#define TIMELIMIT 60


pthread_t time_thread;
pthread_t input_thread;
WINDOW* win;

char databaseKata[WORDSCOUNT][MAXWORDLENGTH]; //database

int countWrongLetterTyped = 0;
int countCorrectLetterTyped = 0;

time_t starting_time;
time_t current_time;

char *menu_choices[] = {
        "Singleplayer",
        "Multiplayer",
        "Exit"
    };

typedef struct {
    // seed
    // enemy score
} server_data;
int choices_count = sizeof(menu_choices) / sizeof(char*);

int typedLetterIndex = 0;
char typedLetter[255] = {0};
char typeThis[255];

char ch;
int timeSpent = 0;
void print_menu(WINDOW* menu, int highlighted);

bool not_playing = true;

int menu();
int main();