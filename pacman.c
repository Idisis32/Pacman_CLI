#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>

#define WIDTH 20
#define HEIGHT 15
#define GHOST_COUNT 3

typedef struct {
    int x, y;
    int dx, dy;
    char symbol;
} Character;

// Function prototypes
void drawMenu();
void drawGame();
void initGame();
void movePacman();
void moveGhosts();
void drawMap();
int checkCollision();
void showGameOver();
void gotoxy(int x, int y);
void hideCursor();
void setColor(int color);

// Global variables
char map[HEIGHT][WIDTH];
Character pacman;
Character ghosts[GHOST_COUNT];
int score = 0;
int lives = 3;
bool gameRunning = false;
bool inMenu = true;

int main() {
    srand(time(NULL));
    hideCursor();
    
    while (1) {
        if (inMenu) {
            drawMenu();
            
            char input = getch();
            if (input == '1') {
                inMenu = false;
                gameRunning = true;
                initGame();
            } else if (input == '2') {
                system("cls");
                printf("\n Controls:\n");
                printf(" W - Move Up\n");
                printf(" S - Move Down\n");
                printf(" A - Move Left\n");
                printf(" D - Move Right\n");
                printf(" ESC - Exit to Menu\n");
                printf("\n Press any key to return...");
                getch();
            } else if (input == '3' || input == 27) {
                system("cls");
                printf("\n Exiting game...\n");
                return 0;
            }
        } else if (gameRunning) {
            drawGame();
            
            if (kbhit()) {
                char key = getch();
                if (key == 27) { // ESC
                    gameRunning = false;
                    inMenu = true;
                    continue;
                }
                
                // Pacman controls
                switch (key) {
                    case 'w': case 'W':
                        pacman.dx = 0; pacman.dy = -1;
                        break;
                    case 's': case 'S':
                        pacman.dx = 0; pacman.dy = 1;
                        break;
                    case 'a': case 'A':
                        pacman.dx = -1; pacman.dy = 0;
                        break;
                    case 'd': case 'D':
                        pacman.dx = 1; pacman.dy = 0;
                        break;
                }
            }
            
            movePacman();
            moveGhosts();
            
            int collision = checkCollision();
            if (collision == 1) { // Collision with ghost
                lives--;
                if (lives <= 0) {
                    showGameOver();
                    gameRunning = false;
                    inMenu = true;
                } else {
                    // Reset positions
                    pacman.x = 1;
                    pacman.y = 1;
                    pacman.dx = pacman.dy = 0;
                }
            }
            
            Sleep(150); // Delay for game speed
        }
    }
    
    return 0;
}

void drawMenu() {
    system("cls");
    setColor(14); // Yellow
    
    // ASCII art title (PowerShell compatible)
    printf("\n");
    printf("  *************************************\n");
    printf("  *       P A C - M A N   G A M E     *\n");
    printf("  *************************************\n");
    
    setColor(11); // Cyan
    printf("\n");
    printf("            +----------------------+\n");
    printf("            |     MAIN MENU        |\n");
    printf("            +----------------------+\n");
    printf("\n");
    
    setColor(15); // White
    printf("                [1] START GAME\n");
    printf("                [2] CONTROLS\n");
    printf("                [3] EXIT\n");
    printf("\n");
    printf("            SELECT MENU ITEM: ");
    
    setColor(10); // Green
}

void initGame() {
    score = 0;
    lives = 3;
    
    // Initialize map
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                map[y][x] = '#'; // Walls
            } else if (rand() % 100 < 70 && !(x == 1 && y == 1)) {
                map[y][x] = '.'; // Food
            } else {
                map[y][x] = ' '; // Empty space
            }
        }
    }
    
    // Add passages
    for (int i = 5; i < HEIGHT - 5; i++) {
        map[i][10] = ' ';
    }
    
    // Initialize pacman
    pacman.x = 1;
    pacman.y = 1;
    pacman.dx = pacman.dy = 0;
    pacman.symbol = 'C'; // Using 'C' instead of circle for PowerShell compatibility
    
    // Initialize ghosts
    for (int i = 0; i < GHOST_COUNT; i++) {
        ghosts[i].x = WIDTH - 2;
        ghosts[i].y = HEIGHT - 2 - i * 2;
        ghosts[i].dx = -1;
        ghosts[i].dy = 0;
        ghosts[i].symbol = 'G'; // Using 'G' instead of '@'
    }
}

void drawGame() {
    gotoxy(0, 0);
    
    setColor(15); // White
    printf("Score: %d", score);
    printf("     Lives: %d", lives);
    printf("     Controls: WASD, ESC - menu");
    
    // Draw separator line
    setColor(8); // Gray
    printf("\n");
    for (int i = 0; i < 40; i++) {
        printf("-");
    }
    printf("\n");
    
    drawMap();
}

void drawMap() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            gotoxy(x + 2, y + 3);
            
            // Check if pacman is here
            if (x == pacman.x && y == pacman.y) {
                setColor(14); // Yellow
                putchar(pacman.symbol);
                continue;
            }
            
            // Check if ghost is here
            bool ghostHere = false;
            for (int i = 0; i < GHOST_COUNT; i++) {
                if (x == ghosts[i].x && y == ghosts[i].y) {
                    setColor(12); // Red
                    putchar(ghosts[i].symbol);
                    ghostHere = true;
                    break;
                }
            }
            
            if (ghostHere) continue;
            
            // Draw map
            switch (map[y][x]) {
                case '#':
                    setColor(9); // Blue
                    putchar('#');
                    break;
                case '.':
                    setColor(10); // Green
                    putchar('.');
                    break;
                default:
                    putchar(' ');
                    break;
            }
        }
        printf("\n");
    }
}

void movePacman() {
    int newX = pacman.x + pacman.dx;
    int newY = pacman.y + pacman.dy;
    
    // Wall collision check
    if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
        if (map[newY][newX] != '#') {
            pacman.x = newX;
            pacman.y = newY;
            
            // Collect food
            if (map[pacman.y][pacman.x] == '.') {
                map[pacman.y][pacman.x] = ' ';
                score += 10;
            }
        }
    }
}

void moveGhosts() {
    for (int i = 0; i < GHOST_COUNT; i++) {
        // Random direction change
        if (rand() % 100 < 20) {
            ghosts[i].dx = rand() % 3 - 1;
            ghosts[i].dy = rand() % 3 - 1;
            
            // Ensure ghost doesn't stand still
            if (ghosts[i].dx == 0 && ghosts[i].dy == 0) {
                ghosts[i].dx = 1;
            }
        }
        
        int newX = ghosts[i].x + ghosts[i].dx;
        int newY = ghosts[i].y + ghosts[i].dy;
        
        // Wall and boundary check
        if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
            if (map[newY][newX] != '#') {
                ghosts[i].x = newX;
                ghosts[i].y = newY;
            } else {
                // If hit wall - change direction
                ghosts[i].dx = -ghosts[i].dx;
                ghosts[i].dy = -ghosts[i].dy;
            }
        } else {
            // If out of bounds - change direction
            ghosts[i].dx = -ghosts[i].dx;
            ghosts[i].dy = -ghosts[i].dy;
        }
    }
}

int checkCollision() {
    // Check collision with ghosts
    for (int i = 0; i < GHOST_COUNT; i++) {
        if (pacman.x == ghosts[i].x && pacman.y == ghosts[i].y) {
            return 1; // Collision with ghost
        }
    }
    return 0; // No collision
}

void showGameOver() {
    system("cls");
    setColor(12); // Red
    
    // ASCII art for GAME OVER
    printf("\n");
    printf("   ***********************************\n");
    printf("   *         G A M E   O V E R      *\n");
    printf("   ***********************************\n");
    
    setColor(15); // White
    printf("\n                     GAME OVER!\n");
    printf("                     Your Score: %d\n", score);
    printf("\n              Press any key to continue...");
    
    getch();
}

// Helper functions
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