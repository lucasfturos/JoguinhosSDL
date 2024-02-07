#include "init_sdl.h"

int init(Resources *res) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    res->win = SDL_CreateWindow("Flappy Bird Remix", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                                SDL_WINDOW_VULKAN);
    if (res->win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        destroyResources(res);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    res->ren = SDL_CreateRenderer(
        res->win, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    if (res->ren == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        destroyResources(res);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "Erro ao inicializar SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    res->font = TTF_OpenFont("./res/font/flappybird.ttf", 55);
    if (res->font == NULL) {
        fprintf(stderr, "Erro ao carregar a fonte: %s\n", TTF_GetError());
        destroyResources(res);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    res->bgTex = IMG_LoadTexture(res->ren, "./res/img/background.png");
    res->birdTex = IMG_LoadTexture(res->ren, "./res/img/flappy.png");
    res->pipeTex = IMG_LoadTexture(res->ren, "./res/img/pipe.png");
    if (res->bgTex == NULL || res->pipeTex == NULL || res->birdTex == NULL) {
        fprintf(stderr, "Erro ao carregar texturas: %s\n", IMG_GetError());
        destroyResources(res);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void destroyResources(Resources *res) {
    TTF_CloseFont(res->font);
    SDL_DestroyTexture(res->bgTex);
    SDL_DestroyTexture(res->pipeTex);
    SDL_DestroyTexture(res->birdTex);
    SDL_DestroyRenderer(res->ren);
    SDL_DestroyWindow(res->win);
}
