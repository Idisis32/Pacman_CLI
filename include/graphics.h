#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "config.h"
#include "game.h"

// Цвета (палитра для Pac-Man)
typedef enum {
    COLOR_BLACK = 0,
    COLOR_DARK_BLUE,
    COLOR_DARK_GREEN,
    COLOR_DARK_CYAN,
    COLOR_DARK_RED,
    COLOR_DARK_MAGENTA,
    COLOR_DARK_YELLOW,
    COLOR_GRAY,
    COLOR_DARK_GRAY,
    COLOR_BLUE,
    COLOR_GREEN,
    COLOR_CYAN,
    COLOR_RED,
    COLOR_MAGENTA,
    COLOR_YELLOW,
    COLOR_WHITE,
    COLOR_PACMAN = COLOR_YELLOW,
    COLOR_BLINKY = COLOR_RED,
    COLOR_PINKY = COLOR_MAGENTA,
    COLOR_INKY = COLOR_CYAN,
    COLOR_CLYDE = COLOR_DARK_YELLOW,
    COLOR_FRIGHTENED = COLOR_BLUE,
    COLOR_DOT = COLOR_WHITE,
    COLOR_POWERUP = COLOR_GREEN,
    COLOR_WALL = COLOR_BLUE,
    COLOR_FRUIT = COLOR_DARK_RED,
    COLOR_TEXT = COLOR_WHITE,
    COLOR_SCORE = COLOR_YELLOW,
    COLOR_LIVES = COLOR_RED,
    COLOR_LEVEL = COLOR_GREEN
} ConsoleColor;

// Графические примитивы
typedef struct {
    int x, y;
    char ch;
    ConsoleColor fg;
    ConsoleColor bg;
} Pixel;

typedef struct {
    Pixel* buffer;
    int width;
    int height;
    int cursor_x;
    int cursor_y;
    ConsoleColor current_fg;
    ConsoleColor current_bg;
} FrameBuffer;

// Окна
typedef struct {
    int x, y;
    int width, height;
    char* title;
    int border;
    ConsoleColor border_color;
    ConsoleColor bg_color;
} Window;

// Анимации
typedef struct {
    char* frames;
    int frame_count;
    int current_frame;
    int frame_delay;
    int frame_timer;
    int loop;
    int active;
} Animation;

// Графический движок
typedef struct {
    FrameBuffer* framebuffer;
    Window* windows[10];
    int window_count;
    Animation* animations[20];
    int animation_count;
    HANDLE console_handle;
    int double_buffer;
    FrameBuffer* back_buffer;
} GraphicsEngine;

// Инициализация
GraphicsEngine* graphics_init(int width, int height, int double_buffer);
void graphics_shutdown(GraphicsEngine* engine);

// Отрисовка
void graphics_clear(GraphicsEngine* engine);
void graphics_present(GraphicsEngine* engine);
void graphics_draw_pixel(GraphicsEngine* engine, int x, int y, char ch, 
                        ConsoleColor fg, ConsoleColor bg);
void graphics_draw_text(GraphicsEngine* engine, int x, int y, 
                       const char* text, ConsoleColor color);
void graphics_draw_box(GraphicsEngine* engine, Window* window);

// Окна
Window* graphics_create_window(GraphicsEngine* engine, int x, int y, 
                              int width, int height, const char* title);
void graphics_draw_window(GraphicsEngine* engine, Window* window);

// Анимации
Animation* graphics_create_animation(GraphicsEngine* engine, 
                                    const char* frames, int delay, int loop);
void graphics_update_animations(GraphicsEngine* engine);
void graphics_draw_animation(GraphicsEngine* engine, Animation* anim, 
                            int x, int y, ConsoleColor color);

// Специализированная отрисовка игры
void graphics_draw_maze(GraphicsEngine* engine, Maze* maze);
void graphics_draw_player(GraphicsEngine* engine, Player* player);
void graphics_draw_ghosts(GraphicsEngine* engine, Ghost* ghosts, int count);
void graphics_draw_ui(GraphicsEngine* engine, Game* game);
void graphics_draw_hud(GraphicsEngine* engine, Game* game);

#endif