#ifndef FLAPPY_H
#define FLAPPY_H

#ifdef __EMSCRIPTEN__
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Macros
#define FPS 60

#define WIDTH 1080
#define HEIGHT 640
#define BIRD_WIDTH 34
#define BIRD_HEIGHT 24
#define NUM_FRAMES 3
#define PIPE_WIDTH 52
#define PIPE_HEIGHT 320
#define PIPE_SPEED 5
#define MAX_PIPES 5

// Structs
typedef struct Pipe {
    int x;
    int topY;
    int bottomY;
    int passed;
    struct Pipe *next;
} Pipe;

typedef struct {
    SDL_Window *win;
    SDL_Renderer *ren;
    TTF_Font *font;
    SDL_Texture *bgTex;
    SDL_Texture *pipeTex;
    SDL_Texture *birdTex;
} Resources;

// Enum
enum GameState {
    PLAYING,
    PAUSED,
    DEFEAT,
};

#endif // !FLAPPY_H
