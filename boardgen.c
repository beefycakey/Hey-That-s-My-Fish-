/* boardgen.c */
#include "boardgen.h"

void saveGame(GameData *game, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file for saving.\n");
        return;
    }

    fprintf(file, "%d %d\n", game->m, game->n);
    fprintf(file, "%d %d\n", game->numplayers, game->numpenguins);
    fprintf(file, "%d\n", game->currentplayer);
    for (int i = 0; i < game->numplayers; i++) {
        fprintf(file, "%d ", game->scores[i]);
    }
    fprintf(file, "\n");

    for (int i = 0; i < game->m; i++) {
        for (int j = 0; j < game->n; j++) {
            fprintf(file, "%c ", game->board[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Game saved successfully to %s.\n", filename);
}

void allocateBoard(GameData *game) {
    game->board = malloc(game->m * sizeof(char *));
    if (!game->board) {
        printf("Memory allocation failed. Exiting.\n");
        exit(1);
    }
    for (int i = 0; i < game->m; i++) {
        game->board[i] = malloc(game->n * sizeof(char));
        if (!game->board[i]) {
            printf("Memory allocation failed. Exiting.\n");
            exit(1);
        }
    }
}

void freeBoard(GameData *game) {
    for (int i = 0; i < game->m; i++) {
        free(game->board[i]);
    }
    free(game->board);
}

void freeBoardWrapper() {
    freeBoard(globalGame);
}

void loadGame(GameData *game, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file for loading.\n");
        return;
    }

    if (game->board) {
        freeBoard(game);
    }

    fscanf(file, "%d %d", &game->m, &game->n);
    fscanf(file, "%d %d", &game->numplayers, &game->numpenguins);
    fscanf(file, "%d", &game->currentplayer);
    for (int i = 0; i < game->numplayers; i++) {
        fscanf(file, "%d", &game->scores[i]);
    }

    allocateBoard(game);
    for (int i = 0; i < game->m; i++) {
        for (int j = 0; j < game->n; j++) {
            fscanf(file, " %c", &game->board[i][j]);
        }
    }

    fclose(file);
    printf("Game loaded successfully from %s.\n", filename);

    printf("Board after loading:\n");
    boardgen(game);
}


void initializeBoard(GameData *game) {
    srand(time(NULL));
    //int totalTiles = game->m * game->n;//
    int requiredTiles = game->numplayers * game->numpenguins;
    int oneCount = 0;
    for (int i = 0; i < game->m; i++) {
        for (int j = 0; j < game->n; j++) {
            double randomFactor = (double)rand() / RAND_MAX;
            if (oneCount < requiredTiles) {
                game->board[i][j] = '1';
                oneCount++;
            } else if (randomFactor < 0.1) {
                game->board[i][j] = '0';
            } else if (randomFactor > 0.7) {
                game->board[i][j] = '3';
            } else if (randomFactor > 0.4) {
                game->board[i][j] = '2';
            } else {
                game->board[i][j] = '1';
            }
        }
    }
    while (oneCount < requiredTiles) {
        int row = rand() % game->m;
        int col = rand() % game->n;
        if (game->board[row][col] != '1') {
            game->board[row][col] = '1';
            oneCount++;
        }
    }
}

void boardgen(GameData *game) {
    printf("   ");
    for (int j = 0; j < game->n; j++) {
        printf("%4d", j + 1);
    }
    printf("\n   ");
    for (int j = 0; j < game->n; j++) {
        printf("----");
    }
    printf("\n");
    for (int i = 0; i < game->m; i++) {
        printf("%4d|", i + 1);
        for (int j = 0; j < game->n; j++) {
            printf("%4c", game->board[i][j]);
        }
        printf("\n");
    }
}

void printScores(GameData *game) {
    printf("\nCurrent scoreboard:\n");
    for (int i = 0; i < game->numplayers; i++) {
        printf("Player %d: %d\n", i + 1, game->scores[i]);
    }
    printf("\n");
}
