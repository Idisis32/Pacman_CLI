#ifndef ENTITIES_H
#define ENTITIES_H

#include "config.h"

typedef struct {
    int x, y;
    int dx, dy;
    int next_dx, next_dy; // Очередное направление
    int lives;
    int score;
    int power_mode;
    int power_timer;
    int dots_eaten;
    char symbol;
    int animation_frame;
} Player;

typedef struct {
    int x, y;
    int dx, dy;
    GhostType type;
    int state; // 0: обычный, 1: испуганный, 2: глаза (возвращение в дом)
    int state_timer;
    int speed;
    char symbol;
    int target_x, target_y;
    int scatter_mode;
    int scatter_timer;
} Ghost;

typedef struct {
    int x, y;
    int type; // 0: точка, 1: супер-точка, 2: фрукт
    int value;
    int visible;
    int timer;
} PowerUp;

typedef struct {
    Player player;
    Ghost ghosts[GHOST_COUNT];
    PowerUp *powerups;
    int powerup_count;
    int max_powerups;
} EntityManager;

// Функции управления сущностями
EntityManager* entity_manager_create();
void entity_manager_destroy(EntityManager* manager);
void update_player(EntityManager* manager, int input);
void update_ghosts(EntityManager* manager);
void check_collisions(EntityManager* manager);
void spawn_fruit(EntityManager* manager, int level);

#endif