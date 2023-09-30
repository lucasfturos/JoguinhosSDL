#include "cobrinha.h"

SDL_Rect createRect(int x, int y, int w, int h) {
    SDL_Rect rect = {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
    };
    return rect;
}
