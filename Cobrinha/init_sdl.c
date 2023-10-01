#include "init_sdl.h"

int init(SDL_Window **win, SDL_Renderer **ren, TTF_Font **font) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    *win = SDL_CreateWindow("Cobrinha", SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                           SDL_WINDOW_VULKAN);
    if (*win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        destroyResources(*win, NULL, NULL);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    *ren = SDL_CreateRenderer(*win, -1,
                             SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    if (*ren == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        destroyResources(*win, *ren, NULL);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "Erro ao inicializar SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    *font = TTF_OpenFont("font/times.ttf", 30);

    return EXIT_SUCCESS;
}

void destroyResources(SDL_Window *win, SDL_Renderer *ren, TTF_Font *font) {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
}
