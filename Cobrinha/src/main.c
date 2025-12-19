#include "include/init_sdl.h"
#include "include/obj.h"

int tailSize;
int elapsedTime;
int score, level;
int minutes, seconds;
int boardx = (WIDTH - BOARD_W) / 2;
int boardy = (HEIGHT - BOARD_H) / 2;

int quit = 0;

Point snake, apple;
Point tail[MAX_SIZE_TAIL];

enum SnakeDir currentDir = LEFT;
enum GameState gameState = PLAYING;

const Uint32 frameDelay = 1000 / FPS;
Uint32 lastTime = 0;

void setup(void) {
    srand(time(0));

    score = 0;
    level = 1;
    minutes = 0;
    seconds = 0;
    tailSize = 0;
    elapsedTime = 0;

    snake.x = BOARD_W / 2;
    snake.y = BOARD_H / 2;

    apple.x = rand() % (BOARD_W - boardx) + boardx;
    apple.y = rand() % (BOARD_H - boardy) + boardy;
}

int snakeMunchedTail(void) {
    for (int i = 0; i < tailSize; i++) {
        if (snake.x == tail[i].x && snake.y == tail[i].y)
            return 1;
    }
    return 0;
}

void update(void) {
    int prevX = snake.x;
    int prevY = snake.y;

    switch (currentDir) {
    case UP:
        snake.y -= SPEED_SNAKE - 5;
        break;
    case DOWN:
        snake.y += SPEED_SNAKE - 5;
        break;
    case LEFT:
        snake.x -= SPEED_SNAKE - 5;
        break;
    case RIGHT:
        snake.x += SPEED_SNAKE - 5;
        break;
    }

    if (snake.x < boardx + 10)
        snake.x = BOARD_W - SIZE_SNAKE + 20;
    if (snake.y < boardy + 10)
        snake.y = BOARD_H - SIZE_SNAKE + 45;
    if (snake.x >= BOARD_W)
        snake.x = boardx + 10;
    if (snake.y >= BOARD_H + 20)
        snake.y = boardy + 10;

    if (tailSize > 0) {
        tail[0].x = prevX;
        tail[0].y = prevY;

        for (int i = 1; i < tailSize; i++) {
            int tx = tail[i].x;
            int ty = tail[i].y;
            tail[i].x = prevX;
            tail[i].y = prevY;
            prevX = tx;
            prevY = ty;
        }
    }

    if (snakeMunchedTail()) {
        gameState = DEFEAT;
        setup();
        return;
    }

    if (snake.x + SIZE_SNAKE >= apple.x - 10 &&
        snake.x <= apple.x + SIZE_APPLE &&
        snake.y + SIZE_SNAKE >= apple.y - 10 &&
        snake.y <= apple.y + SIZE_APPLE) {

        score += 5;
        if (score % 20 == 0)
            level++;

        apple.x = rand() % (BOARD_W - boardx) + boardx;
        apple.y = rand() % (BOARD_H - boardy) + boardy;

        if (tailSize < MAX_SIZE_TAIL)
            tail[tailSize++] = snake;
    }
}

void render(Resources *res) {
    drawBoard(res->ren, boardx, boardy, 5);

    char text[64];

    snprintf(text, sizeof(text), "Score: %d", score);
    drawText(res->ren, res->font, boardx, 10, text);

    snprintf(text, sizeof(text), "Level: %d", level);
    drawText(res->ren, res->font, BOARD_W - 140, 10, text);

    snprintf(text, sizeof(text), "Time: %02d:%02d", minutes, seconds);
    drawText(res->ren, res->font, WIDTH / 2 - 60, HEIGHT - 50, text);

    drawApple(res->ren, apple.x, apple.y);
    drawSnake(res->ren, snake.x, snake.y);

    for (int i = 0; i < tailSize; i++)
        drawSnake(res->ren, tail[i].x, tail[i].y);
}

void gameLoop(void *arg) {
    Resources *res = (Resources *)arg;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            quit = 1;

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_w:
            case SDLK_UP:
                currentDir = UP;
                break;
            case SDLK_s:
            case SDLK_DOWN:
                currentDir = DOWN;
                break;
            case SDLK_a:
            case SDLK_LEFT:
                currentDir = LEFT;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                currentDir = RIGHT;
                break;
            case SDLK_SPACE:
                if (gameState == DEFEAT) {
                    gameState = PLAYING;
                    setup();
                } else {
                    gameState = gameState == PLAYING ? PAUSED : PLAYING;
                }
                break;
            case SDLK_q:
                quit = 1;
                break;
            }
        }
    }

    SDL_SetRenderDrawColor(res->ren, 0, 0, 0, 255);
    SDL_RenderClear(res->ren);

    Uint32 now = SDL_GetTicks();
    Uint32 delta = now - lastTime;
    lastTime = now;

    if (gameState == PLAYING) {
        elapsedTime += delta;
        minutes = elapsedTime / 60000;
        seconds = (elapsedTime / 1000) % 60;
        update();
    }

    render(res);
    SDL_RenderPresent(res->ren);

#ifndef __EMSCRIPTEN__
    Uint32 frameStart = SDL_GetTicks();
    Uint32 frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < frameDelay)
        SDL_Delay(frameDelay - frameTime);
#endif
}

int main(void) {
    static Resources res;

    init(&res.win, &res.ren, &res.font);
    setup();
    lastTime = SDL_GetTicks();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(gameLoop, &res, 0, 1);
#else
    while (!quit) {
        gameLoop(&res);
    }
#endif

    destroyResources(res.win, res.ren, res.font);
    SDL_Quit();
    return EXIT_SUCCESS;
}
