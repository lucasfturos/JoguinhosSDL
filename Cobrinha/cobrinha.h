#ifndef COBRINHA_H
#define COBRINHA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Macros
#define WIDTH 1080
#define HEIGHT 720
#define BOARD_W 1000
#define BOARD_H 600
#define FPS 60
#define SPEED_SNAKE 10
#define SIZE_SNAKE 30
#define SIZE_APPLE 20
#define MAX_SIZE_TAIL 100

// Structs
typedef struct {
    int x, y;
} Point;

typedef struct {
    SDL_Window *win;
    SDL_Renderer *ren;
    TTF_Font *font;
} Resources;

// Enum
enum SnakeDir {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

enum GameState {
    PLAYING,
    PAUSED,
    DEFEAT,
};

// Variables global
static int tailSize;
static int elapsedTime;
static int score, level;
static int minutes, seconds;
static int boardx = (WIDTH - BOARD_W) / 2;
static int boardy = (HEIGHT - BOARD_H) / 2;

// Struct global
static Resources resMgr;
static Point snake, apple;
static Point tail[MAX_SIZE_TAIL];

// Enum global
static enum SnakeDir currentDir = LEFT;
static enum GameState gameState = PLAYING;

// Functions
void setup();
void update();
void render(SDL_Renderer *ren);
void addTailSegment(int x, int y);

#endif // !COBRINHA_H
