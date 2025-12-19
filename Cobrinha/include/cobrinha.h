#ifndef COBRINHA_H
#define COBRINHA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

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

#endif // !COBRINHA_H
