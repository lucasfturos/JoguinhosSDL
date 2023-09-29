#ifndef OBJECTS_H
#define OBJECTS_H

#include "quebra_bloco.h"

TargetNode *initializeTargets();
void destroyTargets(TargetNode *head);

SDL_Rect createRect(int x, int y, int w, int h);

void drawGameOver(SDL_Renderer *renderer, TTF_Font *font);
void drawCircle(SDL_Renderer *renderer, float x, float y, int radius,
                SDL_Color color);

void drawScore(SDL_Renderer *renderer, TTF_Font *font, int score,
               SDL_Texture *bgScore);

#endif // !OBJECTS_H
