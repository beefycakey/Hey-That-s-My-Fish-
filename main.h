/* main.h */
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_PLAYERS 4
#define MAX_BOARD_SIZE 200
#define MAX_PENGUINS 3
#define MIN_BOARD_DIM 4
#define MIN_PENGUINS 1

typedef struct {
    int m, n;
    int numplayers;
    int numpenguins;
    int currentplayer;
    char **board;
    int scores[MAX_PLAYERS];
    int aiEnabled;
} GameData;

extern GameData *globalGame;
void printPenguin();
void freeBoard(GameData *game);
void freeBoardWrapper();
void printPenguin();
void loadGame(GameData *game, const char *filename);


#endif /* MAIN_H */