/* main.c */
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boardgen.h"
#include "interactive.h"
#include "movementph.h"
#include "placementph.h"
#include "ai.h"

GameData *globalGame;

void printPenguin() {
    printf("    _\n");
    printf("  ('v')\n");
    printf(" //-=-\\\n");
    printf(" (\\=/)\n");
    printf("  ^^ ^^ \n");
    printf("Welcome to the penguin game!\n");
}

int main(int argc, char *argv[]) {
    GameData game = {0};
    globalGame = &game;
    atexit(freeBoardWrapper);

    // Command-line mode: Skip interactive mode if arguments are provided
    if (argc >= 2) { 
        if (strcmp(argv[1], "phase=placement") == 0) {
            if (argc < 5) {
                printf("Usage: %s phase=placement penguins=N inputfile outputfile\n", argv[0]);
                return 1;
            }
            loadGame(&game, argv[3]);  
            placement(&game);  
            saveGame(&game, argv[4]);  
            return 0;
        } 
        else if (strcmp(argv[1], "phase=movement") == 0) {
            if (argc < 4) {
                printf("Usage: %s phase=movement inputfile outputfile\n", argv[0]);
                return 1;
            }
            loadGame(&game, argv[2]);
            movement(&game);
            saveGame(&game, argv[3]);
            return 0;
        } 
        else if (strcmp(argv[1], "name") == 0) {
            printf("Player: YourName\n");
            return 0;
        } 
        else {
            printf("Invalid phase argument. Use 'phase=placement' or 'phase=movement'.\n");
            return 1;
        }
    }

    // If no command-line arguments, start interactive mode
    printPenguin();
    
    int choice;
    printf("Enter the game mode:\n1. Interactive Mode\n2. Load Saved Game\n3. AI Mode\n");

if (scanf("%d", &choice) != 1) {
    printf("Invalid input. Restart the program.\n");
    return 1;
}

switch (choice) {
    case 1:
        interactive(&game);
        boardgen(&game);
        placement(&game);
        game.currentplayer = 1;
        movement(&game);
        break;

    case 2: { // *NEW OPTION: Resume Saved Game*
        char filename[256];
        printf("Enter the saved game filename to resume: ");
        scanf("%255s", filename);
        loadGame(&game, filename);
        printf("Game resumed successfully from %s.\n", filename);
        boardgen(&game);
        movement(&game);
        break;
    }
    case 3:
        printf("Starting AI Mode...\n");
        aiMode(&game);
        break;
    default:
        printf("Invalid choice. Restart the program.\n");
        return 1;
 }
    freeBoard(&game);
    return 0;
}