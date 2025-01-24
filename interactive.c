/* interactive.c */
#include "interactive.h"
#include "boardgen.h"


void interactive(GameData *game) {
    printf("Number of players (%d-%d): ", 2, MAX_PLAYERS);
    if (scanf("%d", &game->numplayers) != 1 || game->numplayers < 2 || game->numplayers > MAX_PLAYERS) {
        printf("Invalid input. Exiting.\n");
        exit(1);
    }

    printf("Enter number of penguins per player (%d-%d): ", MIN_PENGUINS, MAX_PENGUINS);
    if (scanf("%d", &game->numpenguins) != 1 || game->numpenguins < MIN_PENGUINS || game->numpenguins > MAX_PENGUINS) {
        printf("Invalid input. Exiting.\n");
        exit(1);
    }

    printf("Enter number of columns (min %d): ", MIN_BOARD_DIM);
    if (scanf("%d", &game->n) != 1 || game->n < MIN_BOARD_DIM) {
        printf("Invalid input. Exiting.\n");
        exit(1);
    }

    printf("Enter number of rows (min %d): ", MIN_BOARD_DIM);
    if (scanf("%d", &game->m) != 1 || game->m < MIN_BOARD_DIM) {
        printf("Invalid input. Exiting.\n");
        exit(1);
    }

    allocateBoard(game);
    initializeBoard(game);
}