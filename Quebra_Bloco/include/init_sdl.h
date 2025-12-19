#ifndef INIT_SDL_H
#define INIT_SDL_H

#include "objects.h"

int init(Resources *res) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return EXIT_FAILURE;

    if (TTF_Init() < 0)
        return EXIT_FAILURE;

    res->win = SDL_CreateWindow("Quebrando Bloquinho", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                                SDL_WINDOW_RESIZABLE);

    if (!res->win)
        return EXIT_FAILURE;

    res->ren = SDL_CreateRenderer(
        res->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!res->ren)
        return EXIT_FAILURE;

    SDL_RenderSetLogicalSize(res->ren, WIDTH, HEIGHT);
    SDL_RenderSetIntegerScale(res->ren, SDL_TRUE);

    res->barTexture = IMG_LoadTexture(res->ren, "res/img/Bar.png");
    res->brickTexture = IMG_LoadTexture(res->ren, "res/img/Black_Brick.png");
    res->bgScore = IMG_LoadTexture(res->ren, "res/img/score_board.png");
    res->bgTexture = IMG_LoadTexture(res->ren, "res/img/background_sky.png");
    res->font = TTF_OpenFont("res/font/times.ttf", 30);

    if (!res->barTexture || !res->brickTexture || !res->bgScore ||
        !res->bgTexture || !res->font)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void destroyResources(Resources *res, TargetNode *targetList) {
    if (res->font)
        TTF_CloseFont(res->font);

    if (res->barTexture)
        SDL_DestroyTexture(res->barTexture);

    if (res->brickTexture)
        SDL_DestroyTexture(res->brickTexture);

    if (res->bgScore)
        SDL_DestroyTexture(res->bgScore);

    if (res->bgTexture)
        SDL_DestroyTexture(res->bgTexture);

    if (res->ren)
        SDL_DestroyRenderer(res->ren);

    if (res->win)
        SDL_DestroyWindow(res->win);

    destroyTargets(targetList);

    TTF_Quit();
    SDL_Quit();
}

#endif // !INIT_SDL_H
