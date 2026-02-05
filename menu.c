#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

// External functions
extern void setColor(int color);
extern void gotoxy(int x, int y);
extern void initGame();
extern void resetGame();

// Объявляем массив уровней как extern
extern Level levels[MAX_LEVELS];

void showGameOver() {
    system("cls");
    setColor(12); // Red
    
    printf("\n");
    printf("   ***********************************\n");
    printf("   *         G A M E   O V E R      *\n");
    printf("   ***********************************\n");
    
    setColor(15); // White
    printf("\n                     GAME OVER!\n");
    printf("                     Your Score: %d\n", score);
    if (currentLevel >= 0 && currentLevel < MAX_LEVELS) {
        printf("                     Level Reached: %s\n", levels[currentLevel].name);
    }
    printf("\n              Press any key to continue...");
    
    _getch();
}

void showLevelComplete() {
    system("cls");
    setColor(10); // Green
    
    printf("\n");
    printf("   ***********************************\n");
    printf("   *    L E V E L   C O M P L E T E  *\n");
    printf("   ***********************************\n");
    
    setColor(15); // White
    printf("\n                     Level %d Complete!\n", currentLevel + 1);
    if (currentLevel >= 0 && currentLevel < MAX_LEVELS) {
        printf("                     Level Name: %s\n", levels[currentLevel].name);
    }
    printf("                     Current Score: %d\n", score);
    printf("                     Lives: %d\n", lives);
    
    if (currentLevel < MAX_LEVELS - 1) {
        printf("\n              Press any key for next level...");
    } else {
        printf("\n              CONGRATULATIONS! YOU BEAT THE GAME!\n");
        printf("                     Final Score: %d\n", score);
        printf("\n              Press any key to continue...");
    }
    
    _getch();
}

void drawMenu() {
    system("cls");
    setColor(14); // Yellow
    
    printf("\n");
    printf("  *************************************\n");
    printf("  *       P A C - M A N   G A M E     *\n");
    printf("  *************************************\n");
    
    setColor(11); // Cyan
    printf("\n");
    printf("            +----------------------+\n");
    printf("            |     MAIN MENU        |\n");
    printf("            +----------------------+\n");
    printf("\n");
    
    setColor(15); // White
    printf("                [1] START GAME\n");
    printf("                [2] CONTINUE GAME\n");
    printf("                [3] LEVEL SELECT\n");
    printf("                [4] CONTROLS\n");
    printf("                [5] EXIT\n");
    printf("\n");
    printf("            SELECT MENU ITEM: ");
    
    setColor(10); // Green
}

void drawLevelSelect() {
    system("cls");
    setColor(14); // Yellow
    
    printf("\n");
    printf("  *************************************\n");
    printf("  *       S E L E C T   L E V E L     *\n");
    printf("  *************************************\n");
    
    setColor(11); // Cyan
    printf("\n");
    printf("            +----------------------+\n");
    printf("            |     LEVELS           |\n");
    printf("            +----------------------+\n");
    printf("\n");
    
    setColor(15); // White
    for (int i = 0; i < MAX_LEVELS; i++) {
        printf("                [%d] %s\n", i + 1, levels[i].name);
    }
    printf("                [0] BACK TO MENU\n");
    printf("\n");
    printf("            SELECT LEVEL: ");
    
    setColor(10); // Green
}

void drawControls() {
    system("cls");
    printf("\n Controls:\n");
    printf(" -------------------------------------\n");
    printf(" W or UP ARROW    - Move Up\n");
    printf(" S or DOWN ARROW  - Move Down\n");
    printf(" A or LEFT ARROW  - Move Left\n");
    printf(" D or RIGHT ARROW - Move Right\n");
    printf(" ESC              - Exit to Menu\n");
    printf(" -------------------------------------\n");
    printf("\n Game Rules:\n");
    printf(" - Collect all dots (.) to complete level\n");
    printf(" - Avoid ghosts (G)\n");
    printf(" - Each dot gives 10 points\n");
    printf(" - You have 3 lives\n");
    printf(" - Complete all %d levels to win\n", MAX_LEVELS);
    printf("\n Press any key to return...");
    _getch();
}

void handleMenuInput() {
    char input = _getch();
    
    switch (input) {
        case '1': // Start new game
            resetGame();
            inMenu = false;
            gameRunning = true;
            break;
            
        case '2': // Continue game
            if (score > 0) {
                inMenu = false;
                gameRunning = true;
            }
            break;
            
        case '3': // Level select
            drawLevelSelect();
            input = _getch();
            if (input >= '1' && input <= '0' + MAX_LEVELS) {
                currentLevel = input - '1';
                resetGame();
                inMenu = false;
                gameRunning = true;
            }
            break;
            
        case '4': // Controls
            drawControls();
            break;
            
        case '5': // Exit
        case 27:  // ESC
            system("cls");
            printf("\n Exiting game...\n");
            exit(0);
            break;
    }
}

void handleGameInput() {
    if (!_kbhit()) return;
    
    char key = _getch();
    
    // Handle arrow keys
    if (key == 0 || key == 224) {
        key = _getch();
        switch (key) {
            case 72: // Up arrow
                pacman.dx = 0; pacman.dy = -1;
                break;
            case 80: // Down arrow
                pacman.dx = 0; pacman.dy = 1;
                break;
            case 75: // Left arrow
                pacman.dx = -1; pacman.dy = 0;
                break;
            case 77: // Right arrow
                pacman.dx = 1; pacman.dy = 0;
                break;
        }
    } else if (key == 27) { // ESC
        gameRunning = false;
        inMenu = true;
    } else {
        // Normal keys
        switch (key) {
            case 'w': case 'W':
                pacman.dx = 0; pacman.dy = -1;
                break;
            case 's': case 'S':
                pacman.dx = 0; pacman.dy = 1;
                break;
            case 'a': case 'A':
                pacman.dx = -1; pacman.dy = 0;
                break;
            case 'd': case 'D':
                pacman.dx = 1; pacman.dy = 0;
                break;
        }
    }
    
    // Функция clearInputBuffer должна быть объявлена в game.h и определена в utils.c
    // clearInputBuffer();
}