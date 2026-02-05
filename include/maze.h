#ifndef MAZE_H
#define MAZE_H

#include "config.h"

#define CELL_WALL '#'
#define CELL_DOT '.'
#define CELL_POWERUP 'o'
#define CELL_FRUIT 'F'
#define CELL_EMPTY ' '
#define CELL_TUNNEL 'T'
#define CELL_GHOST_HOME 'H'

typedef struct {
    char *data;
    int width;
    int height;
    int total_dots;
    int total_powerups;
    int seed;
    int *dot_positions;
    int dot_count;
} Maze;

// Алгоритмы генерации
typedef enum {
    MAZE_ALGO_RECURSIVE = 0,
    MAZE_ALGO_PRIMS,
    MAZE_ALGO_CAVES,     // Клеточные автоматы
    MAZE_ALGO_BSP,       // Binary Space Partitioning
    MAZE_ALGO_COUNT
} MazeAlgorithm;

// Генерация
Maze* maze_generate(int width, int height, MazeAlgorithm algorithm, int seed);
Maze* maze_load(const char* filename);
void maze_save(Maze* maze, const char* filename);
void maze_destroy(Maze* maze);

// Валидация
int maze_validate(Maze* maze);
int maze_find_path(Maze* maze, int start_x, int start_y, int end_x, int end_y);

// Доступ к ячейкам
char maze_get_cell(Maze* maze, int x, int y);
void maze_set_cell(Maze* maze, int x, int y, char value);
int maze_is_walkable(Maze* maze, int x, int y);
int maze_is_intersection(Maze* maze, int x, int y);

// Специальные области
void maze_set_ghost_home(Maze* maze, int x, int y, int radius);
void maze_set_player_start(Maze* maze, int x, int y);
void maze_add_tunnels(Maze* maze);

#endif