/* ai.h */
#ifndef AI_H
#define AI_H

#include "main.h"

void aiPlacePenguins(GameData *game, int player);
int aiMove(GameData *game, int player);
void movementAI(GameData *game);
void aiMode(GameData *game);

#endif