#include "cobrinha.h"

void setup() {
    score = 0;
    snake.x = (int)WIDTH / 2;
    snake.y = (int)HEIGHT / 2;
    defeat = 0;
    apple.x = rand() % WIDTH - 1;
    apple.y = rand() % HEIGHT - 1;
}

int init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    resMgr.win = SDL_CreateWindow("Cobrinha", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                                  SDL_WINDOW_VULKAN);
    if (resMgr.win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        destroyResources();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    resMgr.ren = SDL_CreateRenderer(
        resMgr.win, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    if (resMgr.ren == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        destroyResources();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    return 0;
}

void update() {
    if (snake.x < 0) {
        snake.x = 0;
    }
    if (snake.y < 0) {
        snake.y = 0;
    }
    if (snake.x >= WIDTH - SIZE_SNAKE) {
        snake.x = WIDTH - SIZE_SNAKE;
    }
    if (snake.y >= HEIGHT - SIZE_SNAKE) {
        snake.y = HEIGHT - SIZE_SNAKE;
    }
}

void destroyResources() {
    SDL_DestroyRenderer(resMgr.ren);
    SDL_DestroyWindow(resMgr.win);
}

void render(SDL_Renderer *ren) {
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_Rect appleRect = {
        .x = apple.x,
        .y = apple.y,
        .w = 20,
        .h = 20,
    };
    SDL_RenderFillRect(ren, &appleRect);

    SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
    SDL_Rect snakeRect = {
        .x = snake.x,
        .y = snake.y,
        .w = SIZE_SNAKE,
        .h = SIZE_SNAKE,
    };
    SDL_RenderFillRect(ren, &snakeRect);
}

int main(void) {
    init();
    setup();
    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            event.type == SDL_QUIT ? (quit = 1) : 0;
            switch (event.key.keysym.sym) {
            case 'a':
                snake.x -= 5;
                break;
            case 's':
                snake.y += 5;
                break;
            case 'd':
                snake.x += 5;
                break;
            case 'w':
                snake.y -= 5;
                break;
            case 'q':
                quit = 1;
                break;
            default:
                break;
            }
        }

        update();
        render(resMgr.ren);

        SDL_RenderPresent(resMgr.ren);
        SDL_Delay(1000 / FPS);
    }
    destroyResources();
    SDL_Quit();
    return EXIT_FAILURE;
}
