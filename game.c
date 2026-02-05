#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "game.h"

// Game state variables
char map[HEIGHT][WIDTH];
Character pacman;
Character ghosts[GHOST_COUNT];
int score = 0;
int lives = 3;
int currentLevel = 0;
bool gameRunning = false;
bool inMenu = true;
Level levels[MAX_LEVELS]; // Определяем массив уровней

// External functions
extern void gotoxy(int x, int y);
extern void setColor(int color);
extern void initLevels();

void initGame() {
    score = 0;
    lives = 3;
    currentLevel = 0;
    initLevels(); // Инициализируем уровни перед загрузкой
    loadLevel(currentLevel);
}

void drawGame() {
    gotoxy(0, 0);
    
    setColor(15); // White
    printf("Score: %d", score);
    printf("     Lives: %d", lives);
    printf("     Level: %s", levels[currentLevel].name);
    printf("     Controls: WASD/Arrows");
    
    // Draw separator line
    setColor(8); // Gray
    printf("\n");
    for (int i = 0; i < 50; i++) {
        printf("-");
    }
    printf("\n");
    
    drawMap();
}

void drawMap() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            gotoxy(x + 2, y + 3);
            
            // Check if pacman is here
            if (x == pacman.x && y == pacman.y) {
                setColor(14); // Yellow
                putchar(pacman.symbol);
                continue;
            }
            
            // Check if ghost is here
            bool ghostHere = false;
            for (int i = 0; i < GHOST_COUNT; i++) {
                if (x == ghosts[i].x && y == ghosts[i].y) {
                    setColor(12); // Red
                    putchar(ghosts[i].symbol);
                    ghostHere = true;
                    break;
                }
            }
            
            if (ghostHere) continue;
            
            // Draw map
            switch (map[y][x]) {
                case '#':
                    setColor(9); // Blue
                    putchar('#');
                    break;
                case '.':
                    setColor(10); // Green
                    putchar('.');
                    break;
                default:
                    putchar(' ');
                    break;
            }
        }
        printf("\n");
    }
}

void movePacman() {
    int newX = pacman.x + pacman.dx;
    int newY = pacman.y + pacman.dy;
    
    // Wall collision check
    if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
        if (map[newY][newX] != '#') {
            pacman.x = newX;
            pacman.y = newY;
            
            // Collect food
            if (map[pacman.y][pacman.x] == '.') {
                map[pacman.y][pacman.x] = ' ';
                score += 10;
            }
        }
    }
}

void moveGhosts() {
    for (int i = 0; i < GHOST_COUNT; i++) {
        // Random direction change
        if (rand() % 100 < 20) {
            ghosts[i].dx = rand() % 3 - 1;
            ghosts[i].dy = rand() % 3 - 1;
            
            // Ensure ghost doesn't stand still
            if (ghosts[i].dx == 0 && ghosts[i].dy == 0) {
                ghosts[i].dx = (rand() % 2 == 0) ? 1 : -1;
            }
        }
        
        int newX = ghosts[i].x + ghosts[i].dx;
        int newY = ghosts[i].y + ghosts[i].dy;
        
        // Wall and boundary check
        if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
            if (map[newY][newX] != '#') {
                ghosts[i].x = newX;
                ghosts[i].y = newY;
            } else {
                // If hit wall - change direction
                ghosts[i].dx = -ghosts[i].dx;
                ghosts[i].dy = -ghosts[i].dy;
            }
        } else {
            // If out of bounds - change direction
            ghosts[i].dx = -ghosts[i].dx;
            ghosts[i].dy = -ghosts[i].dy;
        }
    }
}

int checkCollision() {
    // Check collision with ghosts
    for (int i = 0; i < GHOST_COUNT; i++) {
        if (pacman.x == ghosts[i].x && pacman.y == ghosts[i].y) {
            return 1; // Collision with ghost
        }
    }
    return 0; // No collision
}

void updateGame() {
    movePacman();
    moveGhosts();
    
    int collision = checkCollision();
    if (collision == 1) { // Collision with ghost
        lives--;
        if (lives <= 0) {
            gameRunning = false;
            inMenu = true;
        } else {
            // Reset positions
            pacman.x = levels[currentLevel].pacman_start_x;
            pacman.y = levels[currentLevel].pacman_start_y;
            pacman.dx = pacman.dy = 0;
        }
    }
    
    // Check if all food is eaten
    bool foodLeft = false;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] == '.') {
                foodLeft = true;
                break;
            }
        }
        if (foodLeft) break;
    }
    
    if (!foodLeft) {
        if (currentLevel < MAX_LEVELS - 1) {
            nextLevel();
        } else {
            // Game completed
            gameRunning = false;
            inMenu = true;
        }
    }
}

void resetGame() {
    score = 0;
    lives = 3;
    currentLevel = 0;
    loadLevel(currentLevel);
}