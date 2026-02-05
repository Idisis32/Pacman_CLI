#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "game.h"

// External functions
extern void hideCursor();
extern void drawMenu();
extern void handleMenuInput();
extern void handleGameInput();
extern void showGameOver();
extern void showLevelComplete();
extern void initLevels(); // Добавляем прототип

int main() {
    srand(time(NULL));
    hideCursor();
    initLevels(); // Инициализируем уровни в начале программы
    
    while (1) {
        if (inMenu) {
            drawMenu();
            handleMenuInput();
        } else if (gameRunning) {
            handleGameInput();
            
            // Check if game is still running (might have been stopped by ESC)
            if (!gameRunning) {
                inMenu = true;
                continue;
            }
            
            // Функции updateGame и drawGame должны быть объявлены
            // updateGame();
            // drawGame();
            
            // Временно для компиляции - простой движок
            if (_kbhit()) {
                char key = _getch();
                if (key == 27) { // ESC
                    gameRunning = false;
                    inMenu = true;
                }
            }
            
            // Check game state
            if (!gameRunning) {
                if (lives <= 0) {
                    showGameOver();
                } else {
                    showLevelComplete();
                }
                inMenu = true;
            }
            
            Sleep(150); // Game speed
        }
    }
    
    return 0;
}