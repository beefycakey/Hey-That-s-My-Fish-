/* boardgen.h */
#ifndef BOARDGEN_H
#define BOARDGEN_H

#include "main.h"

void allocateBoard(GameData *game);
void initializeBoard(GameData *game);
void boardgen(GameData *game);
void printScores(GameData *game);
void loadGame(GameData *game, const char *filename);
void freeBoard(GameData *game);
void freeBoardWrapper();
void saveGame(GameData *game, const char *filename);
int canMove(GameData *game, int player);
int isPathClear(GameData *game, int startx, int starty, int endx, int endy);

#endif /* BOARDGEN_H */