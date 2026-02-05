#ifndef UTILS_H
#define UTILS_H

#include <time.h>

// Математические функции
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define SIGN(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

// Случайные числа
typedef struct {
    unsigned int seed;
    unsigned int a;
    unsigned int c;
    unsigned int m;
} RandomGenerator;

RandomGenerator* random_create(unsigned int seed);
void random_destroy(RandomGenerator* rng);
int random_int(RandomGenerator* rng, int min, int max);
float random_float(RandomGenerator* rng);
int random_chance(RandomGenerator* rng, float probability);
void random_shuffle(RandomGenerator* rng, int* array, int size);

// Таймеры
typedef struct {
    clock_t start_time;
    clock_t paused_time;
    int is_paused;
} Timer;

Timer* timer_create();
void timer_destroy(Timer* timer);
void timer_start(Timer* timer);
void timer_pause(Timer* timer);
void timer_resume(Timer* timer);
void timer_reset(Timer* timer);
double timer_get_elapsed(Timer* timer); // в секундах
int timer_get_elapsed_ms(Timer* timer);

// Ввод
typedef struct {
    int key;
    int pressed;
    int released;
    int repeat_count;
    double press_time;
} KeyEvent;

typedef struct {
    KeyEvent keys[256];
    int mouse_x;
    int mouse_y;
    int mouse_buttons[3];
    int quit_requested;
} InputState;

InputState* input_create();
void input_destroy(InputState* input);
void input_update(InputState* input);
int input_get_key(InputState* input, int key);
int input_get_key_down(InputState* input, int key);
int input_get_key_up(InputState* input, int key);
void input_wait_for_key(int key);

// Файловый ввод/вывод
int file_exists(const char* filename);
int file_read_all(const char* filename, char** buffer, size_t* size);
int file_write_all(const char* filename, const char* data, size_t size);
int file_append(const char* filename, const char* data, size_t size);
int directory_exists(const char* path);
int create_directory(const char* path);

// Конфигурация
typedef struct {
    char* key;
    char* value;
    struct ConfigEntry* next;
} ConfigEntry;

typedef struct {
    ConfigEntry* entries;
    char* filename;
} ConfigFile;

ConfigFile* config_load(const char* filename);
void config_destroy(ConfigFile* config);
const char* config_get(ConfigFile* config, const char* key);
int config_get_int(ConfigFile* config, const char* key, int default_value);
float config_get_float(ConfigFile* config, const char* key, float default_value);
void config_set(ConfigFile* config, const char* key, const char* value);
void config_save(ConfigFile* config);

// Логирование
typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
} LogLevel;

void log_init(const char* filename, LogLevel level);
void log_message(LogLevel level, const char* format, ...);
void log_close();

#define LOG_DEBUG(...) log_message(LOG_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) log_message(LOG_INFO, __VA_ARGS__)
#define LOG_WARNING(...) log_message(LOG_WARNING, __VA_ARGS__)
#define LOG_ERROR(...) log_message(LOG_ERROR, __VA_ARGS__)
#define LOG_FATAL(...) log_message(LOG_FATAL, __VA_ARGS__)

#endif