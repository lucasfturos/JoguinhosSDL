#ifndef DRAW_H
#define DRAW_H

#include "flappy.h"

void drawPause(Resources *res);
void drawGameover(Resources *res);
void drawScore(Resources *res, int score);
void drawBird(Resources *res, SDL_Point point, float birdAngle);
void drawPipe(Resources *res, SDL_Point point, int pipeRotation);
void drawTextWithOutline(SDL_Renderer *ren, TTF_Font *font, int x, int y,
                         char *text, SDL_Color textColor,
                         SDL_Color outlineColor, int outlineWidth);

#endif// !DRAW_H
