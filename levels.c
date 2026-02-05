#include <string.h>
#include "game.h"

// Объявляем уровни как глобальные переменные
Level levels[MAX_LEVELS];

// Инициализируем уровни при загрузке
void initLevels() {
    // Level 1 - Simple maze
    strcpy(levels[0].name, "EASY MAZE");
    levels[0].pacman_start_x = 1;
    levels[0].pacman_start_y = 1;
    levels[0].ghost_start_x = 18;
    levels[0].ghost_start_y = 13;
    
    const char *level1_layout[HEIGHT] = {
        "####################",
        "#..................#",
        "#.####.###..####.###",
        "#.#  #.#  ## #  #..#",
        "#.#  #.#  ## #  #..#",
        "#.####.###..####.###",
        "#..................#",
        "#.###..####.###.####",
        "#.#  ## #  ## #  #.#",
        "#.#  ## #  ## #  #.#",
        "#.###..####.###.####",
        "#..................#",
        "#.####.###..####.###",
        "#..................#",
        "####################"
    };
    
    for (int y = 0; y < HEIGHT; y++) {
        strcpy(levels[0].layout[y], level1_layout[y]);
    }
    
    // Level 2 - Medium maze
    strcpy(levels[1].name, "MEDIUM MAZE");
    levels[1].pacman_start_x = 2;
    levels[1].pacman_start_y = 2;
    levels[1].ghost_start_x = 17;
    levels[1].ghost_start_y = 12;
    
    const char *level2_layout[HEIGHT] = {
        "####################",
        "# ##..##..##..##.. #",
        "# ##..##..##..##.. #",
        "# ..................",
        "# ##..##..##..##.. #",
        "# ##..##..##..##.. #",
        "#..................#",
        "# # # # # # # # # # #",
        "#..................#",
        "# ##..##..##..##.. #",
        "# ##..##..##..##.. #",
        "#..................#",
        "# ##..##..##..##.. #",
        "# ##..##..##..##.. #",
        "####################"
    };
    
    for (int y = 0; y < HEIGHT; y++) {
        strcpy(levels[1].layout[y], level2_layout[y]);
    }
    
    // Level 3 - Hard maze
    strcpy(levels[2].name, "HARD MAZE");
    levels[2].pacman_start_x = 1;
    levels[2].pacman_start_y = 13;
    levels[2].ghost_start_x = 18;
    levels[2].ghost_start_y = 1;
    
    const char *level3_layout[HEIGHT] = {
        "####################",
        "# # # # # # # # # # #",
        "# # # # # # # # # # #",
        "# # # # # # # # # # #",
        "#..................#",
        "# # # # # # # # # # #",
        "# # # # # # # # # # #",
        "# # # # # # # # # # #",
        "#..................#",
        "# # # # # # # # # # #",
        "# # # # # # # # # # #",
        "# # # # # # # # # # #",
        "#..................#",
        "# # # # # # # # # # #",
        "####################"
    };
    
    for (int y = 0; y < HEIGHT; y++) {
        strcpy(levels[2].layout[y], level3_layout[y]);
    }
}

void loadLevel(int level) {
    if (level < 0 || level >= MAX_LEVELS) return;
    
    currentLevel = level;
    
    // Copy level layout to map
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            char cell = levels[level].layout[y][x];
            if (cell == '.' || cell == ' ') {
                map[y][x] = '.'; // Food
            } else {
                map[y][x] = cell; // Wall or empty
            }
        }
    }
    
    // Set pacman position
    pacman.x = levels[level].pacman_start_x;
    pacman.y = levels[level].pacman_start_y;
    pacman.dx = pacman.dy = 0;
    pacman.symbol = 'C';
    
    // Set ghosts positions
    for (int i = 0; i < GHOST_COUNT; i++) {
        ghosts[i].x = levels[level].ghost_start_x - i * 2;
        ghosts[i].y = levels[level].ghost_start_y - i;
        ghosts[i].dx = (i % 2 == 0) ? -1 : 1;
        ghosts[i].dy = 0;
        ghosts[i].symbol = 'G';
    }
}

void nextLevel() {
    currentLevel++;
    if (currentLevel >= MAX_LEVELS) {
        // Game completed
        return;
    }
    loadLevel(currentLevel);
}