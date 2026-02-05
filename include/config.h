#ifndef CONFIG_H
#define CONFIG_H

// Размеры окна
#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 35
#define GAME_AREA_WIDTH 25
#define GAME_AREA_HEIGHT 21
#define UI_PANEL_WIDTH 30

// Уровни сложности
typedef enum {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_NORMAL,
    DIFFICULTY_HARD,
    DIFFICULTY_INSANE,
    DIFFICULTY_COUNT
} Difficulty;

// Настройки игры
typedef struct {
    Difficulty difficulty;
    int enable_sound;
    int enable_animations;
    int color_mode; // 0: монохром, 1: 16 цветов, 2: 256 цветов
    char player_name[32];
} GameConfig;

// Константы привидений
typedef enum {
    GHOST_BLINKY = 0,  // Красный - преследователь
    GHOST_PINKY,       // Розовый - загонщик
    GHOST_INKY,        // Голубой - патрульный
    GHOST_CLYDE,       // Оранжевый - случайный
    GHOST_COUNT
} GhostType;

// Состояния игры
typedef enum {
    GAME_STATE_MENU = 0,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER,
    GAME_STATE_LEVEL_COMPLETE,
    GAME_STATE_HIGHSCORE
} GameStateType;

#endif