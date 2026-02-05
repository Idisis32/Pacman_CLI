#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "entities.h"
#include "maze.h"
#include "ai.h"

// Основное состояние игры
typedef struct {
    GameStateType state;
    GameConfig config;
    
    // Уровень
    int current_level;
    int level_score;
    int total_score;
    int lives;
    int high_score;
    
    // Таймеры
    int game_timer;
    int power_timer;
    int fright_timer;
    int fruit_timer;
    
    // Логика уровней
    int dots_for_fruit;
    int ghost_speeds[GHOST_COUNT];
    int fright_duration;
    
    // Менеджеры
    EntityManager* entities;
    Maze* maze;
    AIManager* ai;
    
    // Сохранение состояния
    int paused;
    int game_over;
    int level_complete;
    char level_message[128];
} Game;

// Инициализация и управление
Game* game_create(GameConfig config);
void game_destroy(Game* game);
void game_reset(Game* game);
void game_next_level(Game* game);
void game_save(Game* game, const char* filename);
void game_load(Game* game, const char* filename);

// Основной цикл
void game_update(Game* game, int input);
void game_render(Game* game);
void game_handle_input(Game* game, int key);

// События
void game_on_dot_eaten(Game* game, int x, int y);
void game_on_powerup_eaten(Game* game, int x, int y);
void game_on_ghost_eaten(Game* game, int ghost_index);
void game_on_player_death(Game* game);
void game_on_level_complete(Game* game);

// Утилиты
int game_calculate_score(Game* game, int base_score);
int game_get_ghost_speed(Game* game, int ghost_index, int state);
int game_get_fright_duration(Game* game);

#endif