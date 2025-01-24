/* movementph.h */
#ifndef MOVEMENTPH_H
#define MOVEMENTPH_H

#include "main.h"

int isPathClear(GameData *game, int startx, int starty, int endx, int endy);
int canMove(GameData *game, int player);
void movement(GameData *game);

#endif /* MOVEMENTPH_H */
