#ifndef OBJ_H
#define OBJ_H

#include "cobrinha.h"

void drawText(SDL_Renderer *ren, TTF_Font *font, int x, int y, char *text);
SDL_Rect createRect(int x, int y, int w, int h);
void drawSnake(SDL_Renderer *ren, int x, int y);
void drawApple(SDL_Renderer *ren, int x, int y);
void drawBoard(SDL_Renderer *ren, int boardx, int boardy, int borderThicness);

#endif // !OBJ_H
