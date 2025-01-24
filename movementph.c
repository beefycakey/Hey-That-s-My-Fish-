/* movementph.c */
#include "movementph.h"
#include "boardgen.h"

int isPathClear(GameData *game, int startx, int starty, int endx, int endy) {
    int dx = abs(startx - endx);
    int dy = abs(starty - endy);

    if ((dx > 2 || dy > 2) || (dx != 0 && dy != 0 && dx != dy)) {
        return 0;
    }

    int xStep = (endx > startx) - (endx < startx);
    int yStep = (endy > starty) - (endy < starty);

    int x = startx, y = starty;
    while (x != endx || y != endy) {
        x += xStep;
        y += yStep;

        if (game->board[x - 1][y - 1] == '0' || 
            (game->board[x - 1][y - 1] >= 'A' && game->board[x - 1][y - 1] <= 'Z')) {
            return 0;
        }
    }
    return 1;
}

int canMove(GameData *game, int player) {
    char penguin = 'A' + player - 1;
    for (int i = 0; i < game->m; i++) {
        for (int j = 0; j < game->n; j++) {
            if (game->board[i][j] == penguin) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0)) continue;
                        int ni = i + dx + 1, nj = j + dy + 1;
                        if (ni >= 1 && ni <= game->m && nj >= 1 && nj <= game->n &&
                            game->board[ni - 1][nj - 1] != '0' &&
                            isPathClear(game, i + 1, j + 1, ni, nj)) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void movement(GameData *game) {
     printf("Movement phase. Enter -1 -1 to exit.\n");
    int startx, starty, endx, endy;

    while (1) {
      if (!canMove(game, game->currentplayer)) {
     printf("Player %d cannot move anymore! Saving game before exiting...\n", game->currentplayer);
    
     char filename[256];
     printf("Enter filename to save the game: ");
     scanf("%255s", filename);
    
     saveGame(game, filename);
    
     int maxScore = -1, winner = -1;
     for (int i = 0; i < game->numplayers; i++) {
        if (game->scores[i] > maxScore) {
            maxScore = game->scores[i];
            winner = i + 1;
        }
     }
     printf("Player %d wins with %d fish!\n", winner, maxScore);

     freeBoard(game);
     exit(0);
    }
      printf("Player %d, enter coordinates of the penguin to move (row x column) or enter '-2 -2' to save: ", game->currentplayer);
     if (scanf("%d %d", &startx, &starty) != 2) {
     printf("Invalid input. Try again.\n");
     continue;
     }

    // **NEW FEATURE: Allow Saving**
     if (startx == -2 && starty == -2) {
     char filename[256];
     printf("Enter filename to save the game: ");
     scanf("%255s", filename);
     saveGame(game, filename);
     printf("Game saved successfully to %s. You can resume later.\n", filename);
     return;
    }

        if (startx < 1 || startx > game->m || starty < 1 || starty > game->n ||
            game->board[startx - 1][starty - 1] != 'A' + game->currentplayer - 1) {
            printf("Invalid position. Try again.\n");
            continue;
        }

        printf("Player %d, select destination (row x column): ", game->currentplayer);
        if (scanf("%d %d", &endx, &endy) != 2 || endx == -1 || endy == -1) {
            printf("Exiting game.\n");
            break;
        }
        if (endx < 1 || endx > game->m || endy < 1 || endy > game->n ||
            game->board[endx - 1][endy - 1] == '0' || 
            (game->board[endx - 1][endy - 1] >= 'A' && game->board[endx - 1][endy - 1] <= 'Z') ||
            !isPathClear(game, startx, starty, endx, endy)) {
            printf("Invalid destination. Try again.\n");
            continue;
        }

        int fish = game->board[endx - 1][endy - 1] - '0';
        game->scores[game->currentplayer - 1] += fish;
        game->board[startx - 1][starty - 1] = '0';
        game->board[endx - 1][endy - 1] = 'A' + game->currentplayer - 1;
        printf("Player %d collected %d fish. Current score: %d\n", game->currentplayer, fish, game->scores[game->currentplayer - 1]);
        boardgen(game);
        printScores(game);
        game->currentplayer = game->currentplayer % game->numplayers + 1;
    }
}
