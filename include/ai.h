#ifndef AI_H
#define AI_H

#include "config.h"
#include "entities.h"
#include "maze.h"

// Стратегии привидений
typedef enum {
    AI_CHASE = 0,       // Преследование
    AI_SCATTER,         // Рассеяние
    AI_FRIGHTENED,      // Испуг
    AI_EATEN,           // Съеден (возврат в дом)
    AI_STRATEGY_COUNT
} GhostStrategy;

// Цели для каждого типа привидений
typedef struct {
    int (*get_target_x)(Ghost* ghost, Player* player, Maze* maze);
    int (*get_target_y)(Ghost* ghost, Player* player, Maze* maze);
    int scatter_x;
    int scatter_y;
    int speed_modifier;
} GhostBehavior;

// Менеджер ИИ
typedef struct {
    GhostBehavior behaviors[GHOST_COUNT];
    int global_mode; // 0: scatter, 1: chase
    int mode_timer;
    int mode_cycle[8]; // Цикл режимов: 7s scatter, 20s chase, 7s scatter...
    int current_phase;
    
    // Pathfinding
    int** distance_map;
    int use_astar;
} AIManager;

// Создание и управление
AIManager* ai_manager_create(Maze* maze);
void ai_manager_destroy(AIManager* manager);
void ai_update(AIManager* manager, EntityManager* entities, Maze* maze);

// Алгоритмы поиска пути
int* find_path_astar(Maze* maze, int start_x, int start_y, int end_x, int end_y);
int* find_path_bfs(Maze* maze, int start_x, int start_y, int end_x, int end_y);
void calculate_distance_map(Maze* maze, int start_x, int start_y);

// Стратегии для каждого привидения
int blinky_target_x(Ghost* ghost, Player* player, Maze* maze);
int blinky_target_y(Ghost* ghost, Player* player, Maze* maze);
int pinky_target_x(Ghost* ghost, Player* player, Maze* maze);
int pinky_target_y(Ghost* ghost, Player* player, Maze* maze);
int inky_target_x(Ghost* ghost, Player* player, Maze* maze);
int inky_target_y(Ghost* ghost, Player* player, Maze* maze);
int clyde_target_x(Ghost* ghost, Player* player, Maze* maze);
int clyde_target_y(Ghost* ghost, Player* player, Maze* maze);

#endif