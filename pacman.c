#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 20
#define HEIGHT 15
#define GHOST_COUNT 3
#define MAX_LEVELS 3

// Структуры
typedef struct {
    int x, y;
    int dx, dy;
    int next_dx, next_dy;  // Добавляем следующее направление
    char symbol;
} Character;

typedef struct {
    char layout[HEIGHT][WIDTH + 1];
    int ghost_start_x;
    int ghost_start_y;
    int pacman_start_x;
    int pacman_start_y;
    char name[20];
} Level;

// Глобальные переменные
char map[HEIGHT][WIDTH];
Character pacman;
Character ghosts[GHOST_COUNT];
int score = 0;
int lives = 3;
int currentLevel = 0;
bool gameRunning = false;
bool inMenu = true;
bool gamePaused = false;
Level levels[MAX_LEVELS];

// Прототипы функций
void gotoxy(int x, int y);
void hideCursor();
void setColor(int color);
void initLevels();
void loadLevel(int level);
void initGame();
void drawMap();
void drawGame();
void movePacman();
void moveGhosts();
int checkCollision();
void updateGame();
void resetGame();
void drawMenu();
void handleMenuInput();
void handleGameInput();
void showGameOver();
void showLevelComplete();
void clearInputBuffer();

// ============ УТИЛИТЫ ============
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearInputBuffer() {
    while (_kbhit()) {
        _getch();
    }
}

// ============ УРОВНИ ============
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
        "#.    .   ..    .  #",
        "#.#  #.#  ## #  #..#",
        "#.#  #.#  ## #  #..#",
        "#.####.###..####.###",
        "#..................#",
        "#.   ..    .   .    ",
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
    
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            char cell = levels[level].layout[y][x];
            if (cell == '.' || cell == ' ') {
                map[y][x] = '.';
            } else {
                map[y][x] = cell;
            }
        }
    }
    
    pacman.x = levels[level].pacman_start_x;
    pacman.y = levels[level].pacman_start_y;
    pacman.dx = 0;
    pacman.dy = 0;
    pacman.next_dx = 0;
    pacman.next_dy = 0;
    pacman.symbol = 'C';
    
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
        return;
    }
    loadLevel(currentLevel);
}

// ============ ИГРОВАЯ ЛОГИКА ============
void initGame() {
    score = 0;
    lives = 3;
    currentLevel = 0;
    loadLevel(currentLevel);
}

void resetGame() {
    score = 0;
    lives = 3;
    currentLevel = 0;
    loadLevel(currentLevel);
}

void drawMap() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            gotoxy(x * 2 + 2, y + 4);
            
            if (x == pacman.x && y == pacman.y) {
                setColor(14);
                printf("C ");
                continue;
            }
            
            bool ghostHere = false;
            for (int i = 0; i < GHOST_COUNT; i++) {
                if (x == ghosts[i].x && y == ghosts[i].y) {
                    setColor(12);
                    printf("G ");
                    ghostHere = true;
                    break;
                }
            }
            
            if (ghostHere) continue;
            
            switch (map[y][x]) {
                case '#':
                    setColor(9);
                    printf("# ");
                    break;
                case '.':
                    setColor(10);
                    printf(". ");
                    break;
                default:
                    printf("  ");
                    break;
            }
        }
        printf("\n");
    }
}

void drawGame() {
    gotoxy(0, 0);
    
    setColor(15);
    printf("SCORE: %d", score);
    printf("    LIVES: %d", lives);
    printf("    LEVEL: %d - %s", currentLevel + 1, levels[currentLevel].name);
    
    setColor(8);
    printf("\n");
    for (int i = 0; i < 55; i++) printf("-");
    printf("\n\n");
    
    drawMap();
    
    setColor(7);
    gotoxy(0, HEIGHT + 6);
    printf("CONTROLS: WASD/ARROWS   ESC-MENU   ENTER-SELECT");
    
    if (gamePaused) {
        setColor(14);
        gotoxy(30, 2);
        printf("PAUSED");
    }
}

void movePacman() {
    // Сохраняем текущее направление
    int current_dx = pacman.dx;
    int current_dy = pacman.dy;
    
    // Проверяем, можем ли мы изменить направление на следующее
    if (pacman.next_dx != 0 || pacman.next_dy != 0) {
        int newX = pacman.x + pacman.next_dx;
        int newY = pacman.y + pacman.next_dy;
        
        if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
            if (map[newY][newX] != '#') {
                pacman.dx = pacman.next_dx;
                pacman.dy = pacman.next_dy;
                pacman.next_dx = 0;
                pacman.next_dy = 0;
            }
        }
    }
    
    // Двигаем пакмена
    int newX = pacman.x + pacman.dx;
    int newY = pacman.y + pacman.dy;
    
    if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
        if (map[newY][newX] != '#') {
            pacman.x = newX;
            pacman.y = newY;
            
            if (map[pacman.y][pacman.x] == '.') {
                map[pacman.y][pacman.x] = ' ';
                score += 10;
            }
        } else {
            // Если уперлись в стену - останавливаемся
            pacman.dx = 0;
            pacman.dy = 0;
        }
    }
}

void moveGhosts() {
    for (int i = 0; i < GHOST_COUNT; i++) {
        // Призраки двигаются случайно, но реже меняют направление
        if (rand() % 100 < 15) {
            int attempts = 0;
            bool valid_move = false;
            
            while (!valid_move && attempts < 10) {
                int new_dx = rand() % 3 - 1;
                int new_dy = rand() % 3 - 1;
                
                if (new_dx == 0 && new_dy == 0) {
                    attempts++;
                    continue;
                }
                
                int newX = ghosts[i].x + new_dx;
                int newY = ghosts[i].y + new_dy;
                
                if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
                    if (map[newY][newX] != '#') {
                        ghosts[i].dx = new_dx;
                        ghosts[i].dy = new_dy;
                        valid_move = true;
                    }
                }
                attempts++;
            }
        }
        
        int newX = ghosts[i].x + ghosts[i].dx;
        int newY = ghosts[i].y + ghosts[i].dy;
        
        if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
            if (map[newY][newX] != '#') {
                ghosts[i].x = newX;
                ghosts[i].y = newY;
            } else {
                // Если уперлись в стену - разворачиваемся
                ghosts[i].dx = -ghosts[i].dx;
                ghosts[i].dy = -ghosts[i].dy;
            }
        } else {
            ghosts[i].dx = -ghosts[i].dx;
            ghosts[i].dy = -ghosts[i].dy;
        }
    }
}

int checkCollision() {
    for (int i = 0; i < GHOST_COUNT; i++) {
        if (pacman.x == ghosts[i].x && pacman.y == ghosts[i].y) {
            return 1;
        }
    }
    return 0;
}

void updateGame() {
    if (gamePaused) return;
    
    movePacman();
    moveGhosts();
    
    if (checkCollision()) {
        lives--;
        if (lives <= 0) {
            gameRunning = false;
            inMenu = true;
        } else {
            // Возвращаем пакмена на старт
            pacman.x = levels[currentLevel].pacman_start_x;
            pacman.y = levels[currentLevel].pacman_start_y;
            pacman.dx = 0;
            pacman.dy = 0;
            pacman.next_dx = 0;
            pacman.next_dy = 0;
            
            // Небольшая задержка после потери жизни
            Sleep(500);
        }
    }
    
    // Проверяем, осталась ли еда
    bool foodLeft = false;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] == '.') {
                foodLeft = true;
                break;
            }
        }
        if (foodLeft) break;
    }
    
    if (!foodLeft) {
        if (currentLevel < MAX_LEVELS - 1) {
            showLevelComplete();
            nextLevel();
        } else {
            showLevelComplete();
            gameRunning = false;
            inMenu = true;
        }
    }
}

// ============ МЕНЮ ============
void drawMenu() {
    system("cls");
    setColor(14);
    
    printf("\n");
    printf("  *************************************\n");
    printf("  *       P A C - M A N   G A M E     *\n");
    printf("  *************************************\n");
    
    setColor(11);
    printf("\n");
    printf("            +----------------------+\n");
    printf("            |     MAIN MENU        |\n");
    printf("            +----------------------+\n");
    printf("\n");
    
    setColor(15);
    printf("                [1] START GAME\n");
    printf("                [2] CONTINUE\n");
    printf("                [3] LEVEL SELECT\n");
    printf("                [4] CONTROLS\n");
    printf("                [5] EXIT\n");
    printf("\n");
    printf("            SELECT: ");
    
    setColor(10);
}

void drawLevelSelect() {
    system("cls");
    setColor(14);
    
    printf("\n");
    printf("  *************************************\n");
    printf("  *       S E L E C T   L E V E L     *\n");
    printf("  *************************************\n");
    
    setColor(11);
    printf("\n");
    printf("            +----------------------+\n");
    printf("            |     LEVELS           |\n");
    printf("            +----------------------+\n");
    printf("\n");
    
    setColor(15);
    for (int i = 0; i < MAX_LEVELS; i++) {
        printf("                [%d] %s\n", i + 1, levels[i].name);
    }
    printf("                [0] BACK\n");
    printf("\n");
    printf("            SELECT LEVEL: ");
    
    setColor(10);
}

void drawControls() {
    system("cls");
    setColor(11);
    printf("\n  ================ CONTROLS ================\n\n");
    setColor(15);
    printf("  MOVE UP:        W or UP ARROW\n");
    printf("  MOVE DOWN:      S or DOWN ARROW\n");
    printf("  MOVE LEFT:      A or LEFT ARROW\n");
    printf("  MOVE RIGHT:     D or RIGHT ARROW\n");
    printf("  PAUSE:          P\n");
    printf("  MENU:           ESC\n");
    printf("  SELECT:         ENTER\n\n");
    
    setColor(14);
    printf("  ================ GAME RULES ================\n\n");
    setColor(15);
    printf("  Collect all green dots (.)\n");
    printf("  Avoid red ghosts (G)\n");
    printf("  10 points per dot\n");
    printf("  3 lives\n");
    printf("  Complete all %d levels\n\n", MAX_LEVELS);
    
    setColor(10);
    printf("  Press any key to return...");
    _getch();
}

void showGameOver() {
    system("cls");
    setColor(12);
    
    printf("\n");
    printf("   ***********************************\n");
    printf("   *         G A M E   O V E R      *\n");
    printf("   ***********************************\n");
    
    setColor(15);
    printf("\n                     GAME OVER!\n");
    printf("                     Your Score: %d\n", score);
    if (currentLevel >= 0 && currentLevel < MAX_LEVELS) {
        printf("                     Level: %s\n", levels[currentLevel].name);
    }
    printf("\n              Press any key to continue...");
    
    _getch();
}

void showLevelComplete() {
    system("cls");
    setColor(10);
    
    printf("\n");
    printf("   ***********************************\n");
    printf("   *    L E V E L   C O M P L E T E  *\n");
    printf("   ***********************************\n");
    
    setColor(15);
    printf("\n                     Level %d Complete!\n", currentLevel + 1);
    if (currentLevel >= 0 && currentLevel < MAX_LEVELS) {
        printf("                     %s\n", levels[currentLevel].name);
    }
    printf("                     Score: %d\n", score);
    printf("                     Lives: %d\n", lives);
    
    if (currentLevel < MAX_LEVELS - 1) {
        printf("\n              Press any key for next level...");
    } else {
        printf("\n              CONGRATULATIONS! YOU WIN!\n");
        printf("                     Final Score: %d\n", score);
        printf("\n              Press any key to continue...");
    }
    
    _getch();
}

// ============ ОБРАБОТКА ВВОДА ============
void handleMenuInput() {
    char input = _getch();
    
    switch (input) {
        case '1':
            resetGame();
            inMenu = false;
            gameRunning = true;
            gamePaused = false;
            break;
            
        case '2':
            if (score > 0) {
                inMenu = false;
                gameRunning = true;
                gamePaused = false;
            }
            break;
            
        case '3':
            drawLevelSelect();
            input = _getch();
            if (input >= '1' && input <= '0' + MAX_LEVELS) {
                currentLevel = input - '1';
                resetGame();
                inMenu = false;
                gameRunning = true;
                gamePaused = false;
            }
            break;
            
        case '4':
            drawControls();
            break;
            
        case '5':
        case 27:
            system("cls");
            printf("\n Exiting game...\n");
            exit(0);
            break;
    }
}

void handleGameInput() {
    if (!_kbhit()) return;
    
    char key = _getch();
    
    // Обработка ESC
    if (key == 27) {
        gameRunning = false;
        inMenu = true;
        return;
    }
    
    // Обработка PAUSE
    if (key == 'p' || key == 'P') {
        gamePaused = !gamePaused;
        return;
    }
    
    // Не обрабатываем движение, если игра на паузе
    if (gamePaused) return;
    
    // Обработка стрелок
    if (key == 0 || key == 224) {
        key = _getch();
        switch (key) {
            case 72: // Up arrow
                pacman.next_dx = 0;
                pacman.next_dy = -1;
                break;
            case 80: // Down arrow
                pacman.next_dx = 0;
                pacman.next_dy = 1;
                break;
            case 75: // Left arrow
                pacman.next_dx = -1;
                pacman.next_dy = 0;
                break;
            case 77: // Right arrow
                pacman.next_dx = 1;
                pacman.next_dy = 0;
                break;
        }
    } else {
        // Обработка WASD
        switch (key) {
            case 'w': case 'W':
                pacman.next_dx = 0;
                pacman.next_dy = -1;
                break;
            case 's': case 'S':
                pacman.next_dx = 0;
                pacman.next_dy = 1;
                break;
            case 'a': case 'A':
                pacman.next_dx = -1;
                pacman.next_dy = 0;
                break;
            case 'd': case 'D':
                pacman.next_dx = 1;
                pacman.next_dy = 0;
                break;
        }
    }
}

// ============ ОСНОВНАЯ ФУНКЦИЯ ============
int main() {
    srand(time(NULL));
    hideCursor();
    initLevels();
    
    while (1) {
        if (inMenu) {
            drawMenu();
            handleMenuInput();
            clearInputBuffer();
        } else if (gameRunning) {
            // Обрабатываем ввод
            handleGameInput();
            
            // Обновляем игру
            updateGame();
            
            // Отрисовываем
            drawGame();
            
            Sleep(150); // Скорость игры
        }
    }
    
    return 0;
}