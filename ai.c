/* ai.c */
#include "ai.h"
#include "boardgen.h"
#include "interactive.h"
#include "movementph.h"
#include "placementph.h"


void aiPlacePenguins(GameData *game, int player) {
    for (int p = 0; p < game->numpenguins; p++) {
        int placed = 0;
        for (int i = 0; i < game->m && !placed; i++) {
            for (int j = 0; j < game->n && !placed; j++) {
                if (game->board[i][j] == '1') {
                    game->board[i][j] = 'A' + (player - 1);
                    game->scores[player - 1] += 1; // Collect that 1 fish
                    placed = 1;
                }
            }
        }
        boardgen(game);
        printScores(game);
        if (!placed) {
            // If for some reason there's no place for a penguin
            printf("AI couldn't place a penguin. Board might be full of 0 or higher fish.\n");
        }
    }
}


int aiMove(GameData *game, int player) {
    char penguin = 'A' + player - 1;
    for (int i = 0; i < game->m; i++) {
        for (int j = 0; j < game->n; j++) {
            if (game->board[i][j] == penguin) {
                // Check all possible directions (straight moves only)
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        // We skip (0,0) and diagonal moves
                        if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0)) continue;

                        int ni = i + dx;
                        int nj = j + dy;
                        if (ni >= 0 && ni < game->m && nj >= 0 && nj < game->n) {
                            // Check if it's a valid tile and path is clear
                            if (game->board[ni][nj] != '0' &&
                                !(game->board[ni][nj] >= 'A' && game->board[ni][nj] <= 'Z') &&
                                isPathClear(game, i + 1, j + 1, ni + 1, nj + 1)) {

                                int fish = game->board[ni][nj] - '0';
                                // Move the penguin
                                game->scores[player - 1] += fish;
                                game->board[i][j] = '0';
                                game->board[ni][nj] = penguin;
                                printf("AI (Player %d) moved penguin from (%d,%d) to (%d,%d)\n",
                                        player, i+1, j+1, ni+1, nj+1);
                                printf("AI collected %d fish. Current score: %d\n", 
                                        fish, game->scores[player - 1]);
                                boardgen(game);
                                printScores(game);
                                return 1; // Success
                            }
                        }
                    }
                }
            }
        }
    }
    return 0; // No move found
}


void movementAI(GameData *game) {
    printf("AI Movement phase. Enter -1 -1 at any prompt to exit.\n");
    int startx, starty, endx, endy;

    while (1) {
        // Check if the current player can move
        if (!canMove(game, game->currentplayer)) {
            printf("Player %d cannot move anymore! Game over.\n", game->currentplayer);

            // Determine the winner
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

        // If AI's turn
        if (game->currentplayer == 2 && game->aiEnabled) {
            printf("AI (Player %d) is thinking...\n", game->currentplayer);
            if (!aiMove(game, game->currentplayer)) {
                // If AI can't move, game ends
                printf("AI (Player %d) cannot move anymore! Game over.\n", game->currentplayer);

                // Determine winner
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
            // Switch to next player
            game->currentplayer = game->currentplayer % game->numplayers + 1;
            continue;
        }

        // Otherwise, human's turn
        printf("Player %d, enter coordinates of the penguin to move (row x column): ", game->currentplayer);
        if (scanf("%d %d", &startx, &starty) != 2 || startx == -1 || starty == -1) {
            printf("Exiting game.\n");
            break;
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


void aiMode(GameData *game) {
    printf("AI Mode! You will play against a simple AI.\n");

    // We only allow 2 players: the user and the AI
    game->numplayers = 2;
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

    // Mark AI as enabled
    game->aiEnabled = 1;

    // Allocate & initialize the board
    allocateBoard(game);
    initializeBoard(game);

    // Show the board
    boardgen(game);

    // Placement: human first, then AI
    printf("Placing penguins for Player 1 (Human):\n");
    for (int p = 0; p < game->numpenguins; p++) {
        int x, y;
        printf("Player 1, place your penguin on an ice block with '1' (row x column): ");
        if (scanf("%d %d", &x, &y) != 2 || x < 1 || x > game->m || y < 1 || y > game->n || game->board[x - 1][y - 1] != '1') {
            printf("Invalid input. Try again.\n");
            p--;
            continue;
        }
        game->board[x - 1][y - 1] = 'A';  // 'A' => player 1
        game->scores[0] += 1;
        boardgen(game);
        printScores(game);
    }

    printf("Now AI (Player 2) will place penguins automatically.\n");
    aiPlacePenguins(game, 2);

    // Movement
    // We'll start with Player 1 (human)
    game->currentplayer = 1;
    movementAI(game);
}
