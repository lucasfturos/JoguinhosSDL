#ifndef TETRAMINO_H
#define TETRAMINO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#define WIDTH 720
#define HEIGHT 720
#define FPS 60
#define BOARD_W 480
#define BOARD_H HEIGHT
#define SQUARES 4
#define SHAPES 7
#define SIZE_PIECE 40
#define PADDING_PIECE 45

// Matriz de formas
static int forms[SHAPES][SQUARES] = {
    {1, 3, 5, 7}, // I
    {2, 4, 5, 7}, // Z
    {3, 5, 4, 6}, // S
    {3, 5, 4, 7}, // T
    {2, 3, 5, 7}, // L
    {3, 5, 7, 6}, // J
    {2, 3, 4, 5}, // O
};

typedef struct {
    int x, y;
} Point;

typedef struct {
    SDL_Window *win;
    SDL_Renderer *ren;
    TTF_Font *font;
} Resources;

// Declaração das funções SDL
int init();
void update();
void destroyResources();
void render(SDL_Renderer *ren);
void drawPiece(SDL_Renderer *ren);

static Resources resMgr;
static Point z[SQUARES] /*, k[SQUARES]*/;
static int rotate = 0;
static int pieceX = BOARD_W / 2;
static int pieceY = 0, shape = 0;

static Uint32 red_piece = 255, green_piece = 255, blue_piece = 255;
// static int board[BOARD_H][BOARD_W];

#endif // !TETRAMINO_H
