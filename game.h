#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define WIDTH 20
#define HEIGHT 15
#define GHOST_COUNT 3
#define MAX_LEVELS 3

typedef struct {
    int x, y;
    int dx, dy;
    char symbol;
} Character;

typedef struct {
    char layout[HEIGHT][WIDTH];
    int ghost_start_x;
    int ghost_start_y;
    int pacman_start_x;
    int pacman_start_y;
    char name[20];
} Level;

// Game state
extern char map[HEIGHT][WIDTH];
extern Character pacman;
extern Character ghosts[GHOST_COUNT];
extern int score;
extern int lives;
extern int currentLevel;
extern bool gameRunning;
extern bool inMenu;

// Function prototypes
void initGame();
void drawGame();
void updateGame();
void movePacman();
void moveGhosts();
int checkCollision();
void loadLevel(int level);
void nextLevel();
void resetGame();

#endif