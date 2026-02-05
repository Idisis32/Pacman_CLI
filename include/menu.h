#ifndef MENU_H
#define MENU_H

#include "config.h"
#include "graphics.h"

// Элемент меню
typedef struct {
    char* text;
    void* data;
    int (*action)(void* data);
    int selected;
    int disabled;
    MenuItem* submenu;
    int submenu_count;
} MenuItem;

// Меню
typedef struct {
    char* title;
    MenuItem* items;
    int item_count;
    int selected_index;
    int x, y;
    int width, height;
    Window* window;
    GraphicsEngine* graphics;
} Menu;

// Типы меню
typedef enum {
    MENU_MAIN = 0,
    MENU_PAUSE,
    MENU_SETTINGS,
    MENU_HIGHSCORE,
    MENU_DIFFICULTY,
    MENU_CONTROLS,
    MENU_GRAPHICS,
    MENU_ABOUT,
    MENU_TYPE_COUNT
} MenuType;

// Менеджер меню
typedef struct {
    Menu* menus[MENU_TYPE_COUNT];
    Menu* current_menu;
    Menu* menu_stack[10];
    int stack_top;
    GraphicsEngine* graphics;
} MenuManager;

// Создание меню
MenuManager* menu_manager_create(GraphicsEngine* graphics);
void menu_manager_destroy(MenuManager* manager);

// Навигация
void menu_push(MenuManager* manager, MenuType type);
void menu_pop(MenuManager* manager);
void menu_navigate(MenuManager* manager, int direction);
int menu_select(MenuManager* manager);

// Отрисовка
void menu_render(MenuManager* manager);
void menu_update(MenuManager* manager, int input);

// Фабрика меню
Menu* menu_create_main(GraphicsEngine* graphics);
Menu* menu_create_pause(GraphicsEngine* graphics);
Menu* menu_create_settings(GraphicsEngine* graphics);
Menu* menu_create_highscore(GraphicsEngine* graphics);
Menu* menu_create_difficulty(GraphicsEngine* graphics);

// Действия меню
int menu_action_new_game(void* data);
int menu_action_continue(void* data);
int menu_action_save_game(void* data);
int menu_action_load_game(void* data);
int menu_action_settings(void* data);
int menu_action_highscores(void* data);
int menu_action_about(void* data);
int menu_action_exit(void* data);
int menu_action_set_difficulty(void* data);
int menu_action_toggle_sound(void* data);
int menu_action_toggle_fullscreen(void* data);

#endif