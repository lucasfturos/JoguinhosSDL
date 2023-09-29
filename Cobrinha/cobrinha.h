#ifndef COBRINHA_H
#define COBRINHA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>

// Macros
#define WIDTH 720
#define HEIGHT 720
#define FPS 60
#define SIZE_SNAKE 30
#define SIZE_TAIL 100

// Structs
typedef struct {
    int x, y;
} Point;

typedef struct {
    SDL_Window *win;
    SDL_Renderer *ren;
    TTF_Font *font;
} Resources;

// Variables
static int score, defeat;

static Resources resMgr;
static Point snake, apple;
static Point tail[SIZE_TAIL];

// Functions
int init();
void setup();
void update();
void destroyResources();
void render(SDL_Renderer *ren);

#endif // !COBRINHA_H
