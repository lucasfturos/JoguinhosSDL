#ifndef QUEBRA_BLOCO_H
#define QUEBRA_BLOCO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 1080
#define HEIGHT 720
#define FPS 60
#define BAR_LEN 130
#define BAR_THICCNESS 15
#define TARGET_WIDTH 53
#define TARGET_HEIGHT 30
#define TARGET_PADDING 10
#define TARGET_ROWS 4
#define TARGET_COLS 17
#define TARGET_START_X 10
#define TARGET_START_Y 55
#define NUM_TARGETS (TARGET_ROWS * TARGET_COLS)

typedef struct {
    float x, y;
    float speedX, speedY;
    int radius;
} Coord;

typedef struct {
    float x, y;
} Bar;

typedef struct {
    float x, y;
    int dead;
} Target;

typedef struct TargetNode {
    Target target;
    struct TargetNode *next;
} TargetNode;

typedef struct {
    SDL_Window *win;
    SDL_Renderer *ren;
    SDL_Texture *barTexture;
    SDL_Texture *brickTexture;
    SDL_Texture *bgScore;
    SDL_Texture *bgTexture;
    TTF_Font *font;
} ResMgr;

#endif // !QUEBRA_BLOCO_H
