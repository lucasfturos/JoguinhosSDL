#include "obj.h"

int tailSize = 0;
int score, defeat, level;
int boardx = (WIDTH - BOARD_W) / 2;
int boardy = (HEIGHT - BOARD_H) / 2;

Resources resMgr;
Point snake, apple;
Point tail[MAX_SIZE_TAIL];

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

    if (TTF_Init() < 0) {
        fprintf(stderr, "Erro ao inicializar SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    resMgr.font = TTF_OpenFont("font/times.ttf", 30);

    return EXIT_SUCCESS;
}

void destroyResources() {
    TTF_CloseFont(resMgr.font);
    SDL_DestroyRenderer(resMgr.ren);
    SDL_DestroyWindow(resMgr.win);
}

void setup() {
    srand(time(0));

    score = 0;
    level = 1;
    snake.x = (int)BOARD_W / 2;
    snake.y = (int)BOARD_H / 2;
    defeat = 0;
    apple.x = rand() % (BOARD_W + 1 - boardx) + boardx;
    apple.y = rand() % (BOARD_H + 1 - boardy) + boardy;
}

void update() {
    for (int i = tailSize - 1; i > 0; i--) {
        tail[i] = tail[i - 1];
    }

    if (tailSize > 0) {
        tail[0] = snake;
    }

    if (snake.x < ((WIDTH - BOARD_W) / 2) + 10) {
        snake.x = ((WIDTH - BOARD_W) / 2) + 10;
    }

    if (snake.y < ((HEIGHT - BOARD_H) / 2) + 10) {
        snake.y = ((HEIGHT - BOARD_H) / 2) + 10;
    }

    if (snake.x >= BOARD_W) {
        snake.x = BOARD_W;
    }

    if (snake.y >= BOARD_H + 20) {
        snake.y = BOARD_H + 20;
    }

    if (snake.x + SIZE_SNAKE >= apple.x - 10 && snake.x <= apple.x + 30 &&
        snake.y + SIZE_SNAKE >= apple.y - 10 && snake.y <= apple.y + 30) {
        score += 5;
        if (score % 20 == 0) {
            level++;
        }

        apple.x = rand() % (BOARD_W - boardx) + boardx;
        apple.y = rand() % (BOARD_H - boardy) + boardy;

        if (tailSize < MAX_SIZE_TAIL) {
            tailSize++;
            tail[tailSize - 1] = snake;
        }
    }
}

void render(SDL_Renderer *ren) {
    drawBoard(ren, boardx, boardy, 5);
    drawText(ren, resMgr.font, boardx, score, level);
    drawApple(ren, apple.x, apple.y);
    drawSnake(ren, snake.x, snake.y);
    for (int k = 0; k < tailSize; k++) {
        drawSnake(ren, tail[k].x, tail[k].y);
    }
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
                snake.x -= SPEED_SNAKE;
                break;
            case 's':
                snake.y += SPEED_SNAKE;
                break;
            case 'd':
                snake.x += SPEED_SNAKE;
                break;
            case 'w':
                snake.y -= SPEED_SNAKE;
                break;
            case 'q':
                quit = 1;
                break;
            default:
                break;
            }
        }

        const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
        if (keyboard[SDL_SCANCODE_A] != 0) {
            snake.x -= SPEED_SNAKE;
        }
        if (keyboard[SDL_SCANCODE_D] != 0) {
            snake.x += SPEED_SNAKE;
        }
        if (keyboard[SDL_SCANCODE_W] != 0) {
            snake.y -= SPEED_SNAKE;
        }
        if (keyboard[SDL_SCANCODE_S] != 0) {
            snake.y += SPEED_SNAKE;
        }

        SDL_SetRenderDrawColor(resMgr.ren, 0, 0, 0, 255);
        SDL_RenderClear(resMgr.ren);

        update();
        render(resMgr.ren);

        SDL_RenderPresent(resMgr.ren);
        SDL_Delay(1000 / FPS);
    }
    destroyResources();
    SDL_Quit();
    return EXIT_SUCCESS;
}
