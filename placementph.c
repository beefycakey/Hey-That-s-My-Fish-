/* placementph.c */
#include "placementph.h"
#include "placementph.h"
#include "boardgen.h" 


void placement(GameData *game) {
    printf("Placing penguins\n");
    int x, y;
    for (int p = 0; p < game->numpenguins * game->numplayers; p++) {
        game->currentplayer = p % game->numplayers + 1;
        printf("Player %d, place your penguin on an ice block with '1' (row x column): ", game->currentplayer);
        if (scanf("%d %d", &x, &y) != 2 || x < 1 || x > game->m || y < 1 || y > game->n || game->board[x - 1][y - 1] != '1') {
            printf("Invalid input. Try again.\n");
            p--;
            continue;
        }
        game->board[x - 1][y - 1] = 'A' + game->currentplayer - 1;
        game->scores[game->currentplayer - 1] += 1;
        boardgen(game);
        printScores(game);
    }
}