#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "utils.h"

// Глобальные переменные
GraphicsEngine* graphics = NULL;
MenuManager* menu_manager = NULL;
Game* game = NULL;
InputState* input = NULL;
int running = 1;

// Инициализация
int initialize() {
    // Инициализация логов
    log_init("pacman.log", LOG_INFO);
    LOG_INFO("Pac-Man Console Edition начал работу");
    
    // Инициализация графики
    graphics = graphics_init(CONSOLE_WIDTH, CONSOLE_HEIGHT, 1);
    if (!graphics) {
        LOG_FATAL("Не удалось инициализировать графику");
        return 0;
    }
    
    // Инициализация ввода
    input = input_create();
    if (!input) {
        LOG_FATAL("Не удалось инициализировать ввод");
        return 0;
    }
    
    // Инициализация меню
    menu_manager = menu_manager_create(graphics);
    if (!menu_manager) {
        LOG_FATAL("Не удалось инициализировать меню");
        return 0;
    }
    
    // Загрузка конфигурации
    GameConfig config;
    config.difficulty = DIFFICULTY_NORMAL;
    config.enable_sound = 0;
    config.enable_animations = 1;
    config.color_mode = 1;
    strcpy(config.player_name, "Игрок");
    
    // Создание игры
    game = game_create(config);
    if (!game) {
        LOG_FATAL("Не удалось создать игру");
        return 0;
    }
    
    LOG_INFO("Инициализация завершена успешно");
    return 1;
}

// Основной цикл
void main_loop() {
    Timer* frame_timer = timer_create();
    double target_frame_time = 1.0 / 60.0; // 60 FPS
    double accumulator = 0.0;
    
    // Показ главного меню
    menu_push(menu_manager, MENU_MAIN);
    
    while (running && !input->quit_requested) {
        timer_start(frame_timer);
        
        // Обновление ввода
        input_update(input);
        
        // Проверка на выход
        if (input_get_key(input, VK_ESCAPE) && input_get_key_down(input, VK_ESCAPE)) {
            if (game->state == GAME_STATE_PLAYING) {
                game->state = GAME_STATE_PAUSED;
                menu_push(menu_manager, MENU_PAUSE);
            } else if (menu_manager->current_menu) {
                menu_pop(menu_manager);
            }
        }
        
        // Обработка текущего состояния
        switch (game->state) {
            case GAME_STATE_MENU:
                menu_update(menu_manager, input->key);
                menu_render(menu_manager);
                break;
                
            case GAME_STATE_PLAYING:
                // Обновление игры
                game_update(game, input->key);
                
                // Отрисовка игры
                graphics_clear(graphics);
                game_render(game);
                graphics_present(graphics);
                break;
                
            case GAME_STATE_PAUSED:
                menu_update(menu_manager, input->key);
                menu_render(menu_manager);
                break;
                
            case GAME_STATE_GAME_OVER:
                // Показ экрана Game Over
                graphics_clear(graphics);
                graphics_draw_text(graphics, CONSOLE_WIDTH/2 - 10, CONSOLE_HEIGHT/2 - 2,
                                 "ИГРА ОКОНЧЕНА", COLOR_RED);
                graphics_draw_text(graphics, CONSOLE_WIDTH/2 - 15, CONSOLE_HEIGHT/2,
                                 "Нажмите любую клавишу...", COLOR_TEXT);
                graphics_present(graphics);
                
                if (input_get_key_down(input, VK_RETURN)) {
                    game_reset(game);
                    game->state = GAME_STATE_MENU;
                    menu_push(menu_manager, MENU_MAIN);
                }
                break;
                
            case GAME_STATE_HIGHSCORE:
                // Показ таблицы рекордов
                menu_update(menu_manager, input->key);
                menu_render(menu_manager);
                break;
        }
        
        // Ограничение FPS
        double frame_time = timer_get_elapsed(frame_timer);
        if (frame_time < target_frame_time) {
            DWORD sleep_time = (DWORD)((target_frame_time - frame_time) * 1000);
            if (sleep_time > 0) {
                Sleep(sleep_time);
            }
        }
        
        timer_reset(frame_timer);
    }
    
    timer_destroy(frame_timer);
}

// Очистка
void cleanup() {
    LOG_INFO("Очистка ресурсов...");
    
    if (game) game_destroy(game);
    if (menu_manager) menu_manager_destroy(menu_manager);
    if (graphics) graphics_shutdown(graphics);
    if (input) input_destroy(input);
    
    log_close();
}

// Точка входа
int main() {
    if (!initialize()) {
        MessageBox(NULL, "Ошибка инициализации игры", "Pac-Man", MB_ICONERROR);
        return 1;
    }
    
    main_loop();
    cleanup();
    
    return 0;
}