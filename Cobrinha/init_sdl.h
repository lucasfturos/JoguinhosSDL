#ifndef INIT_SDL_H
#define INIT_SDL_H

#include "cobrinha.h"

int init(SDL_Window **win, SDL_Renderer **ren, TTF_Font **font);
void destroyResources(SDL_Window *win, SDL_Renderer *ren, TTF_Font *font);

#endif // !INIT_SDL_H
