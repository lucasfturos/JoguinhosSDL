#ifndef OBJ_H
#define OBJ_H

#include "cobrinha.h"

static SDL_Rect createRect(int x, int y, int w, int h) {
    SDL_Rect rect = {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
    };
    return rect;
}

static void drawText(SDL_Renderer *ren, TTF_Font *font, int x, int y,
                     char *text) {
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
    SDL_Rect textRect = {
        .x = x,
        .y = y,
        .w = textSurface->w,
        .h = textSurface->h,
    };
    SDL_RenderCopy(ren, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

static void drawSnake(SDL_Renderer *ren, int x, int y) {
    SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
    SDL_Rect snakeRect = createRect(x, y, SIZE_SNAKE, SIZE_SNAKE);
    SDL_RenderFillRect(ren, &snakeRect);
}

static void drawApple(SDL_Renderer *ren, int x, int y) {
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_Rect appleRect = createRect(x, y, SIZE_APPLE, SIZE_APPLE);
    SDL_RenderFillRect(ren, &appleRect);
}

static void drawBoard(SDL_Renderer *ren, int boardx, int boardy,
                      int borderThicness) {
    SDL_Rect top = createRect(boardx, boardy, BOARD_W, borderThicness);
    SDL_Rect bottom = createRect(boardx, boardy + BOARD_H - borderThicness,
                                 BOARD_W, borderThicness);
    SDL_Rect left = createRect(boardx, boardy, borderThicness, BOARD_H);
    SDL_Rect right = createRect(boardx + BOARD_W - borderThicness, boardy,
                                borderThicness, BOARD_H);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderFillRect(ren, &top);
    SDL_RenderFillRect(ren, &bottom);
    SDL_RenderFillRect(ren, &left);
    SDL_RenderFillRect(ren, &right);
}

#endif // !OBJ_H
