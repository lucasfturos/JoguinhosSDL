#include "include/init_sdl.h"
#include "include/obj.h"

// Variables global
int tailSize;
int elapsedTime;
int score, level;
int minutes, seconds;
int boardx = (WIDTH - BOARD_W) / 2;
int boardy = (HEIGHT - BOARD_H) / 2;

// Struct global
Resources resMgr;
Point snake, apple;
Point tail[MAX_SIZE_TAIL];

// Enum global
enum SnakeDir currentDir = LEFT;
enum GameState gameState = PLAYING;

void setup() {
    srand(time(0));

    score = 0;
    level = 1;
    minutes = 0;
    seconds = 0;
    tailSize = 0;
    elapsedTime = 0;
    snake.x = (int)BOARD_W / 2;
    snake.y = (int)BOARD_H / 2;
    apple.x = rand() % (BOARD_W + 10 - boardx) + boardx;
    apple.y = rand() % (BOARD_H + 10 - boardy) + boardy;
}

int snakeMunchedTail() {
    for (int i = 0; i < tailSize; i++) {
        if (snake.x == tail[i].x && snake.y == tail[i].y) {
            return 1;
        }
    }
    return 0;
}

void update() {
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

    if (snake.x < boardx + 10) {
        snake.x = BOARD_W - SIZE_SNAKE + 20;
    }

    if (snake.y < boardy + 10) {
        snake.y = BOARD_H - SIZE_SNAKE + 45;
    }

    if (snake.x >= BOARD_W) {
        snake.x = boardx + 10;
    }

    if (snake.y >= BOARD_H + 20) {
        snake.y = boardy + 10;
    }

    if (tailSize > 0) {
        tail[0].x = prevX;
        tail[0].y = prevY;

        for (int i = 1; i < tailSize; i++) {
            int tempX = tail[i].x;
            int tempY = tail[i].y;
            tail[i].x = prevX;
            tail[i].y = prevY;
            prevX = tempX;
            prevY = tempY;
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
        int noCollisionWithTail = 1;
        for (int i = 0; i < tailSize; i++) {
            if (snake.x == tail[i].x && snake.y == tail[i].y) {
                noCollisionWithTail = 0;
                break;
            }
        }
        if (noCollisionWithTail) {
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
}

void render(SDL_Renderer *ren) {
    drawBoard(ren, boardx, boardy, 5);
    // Score
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
    drawText(ren, resMgr.font, boardx, 10, scoreText);
    // Level
    char levelText[50];
    snprintf(levelText, sizeof(levelText), "Level: %d", level);
    int levelTextWidth;
    TTF_SizeText(resMgr.font, levelText, &levelTextWidth, NULL);
    drawText(ren, resMgr.font, BOARD_W - levelTextWidth + 40, 10, levelText);
    // Counter
    char timeText[50];
    snprintf(timeText, sizeof(timeText), "Time: %02d:%02d", minutes, seconds);
    int textWidth;
    TTF_SizeText(resMgr.font, timeText, &textWidth, NULL);
    drawText(ren, resMgr.font, (WIDTH - textWidth) / 2, HEIGHT - 50, timeText);

    // Objects
    drawApple(ren, apple.x, apple.y);
    drawSnake(ren, snake.x, snake.y);
    for (int k = 0; k < tailSize; k++) {
        drawSnake(ren, tail[k].x, tail[k].y);
    }
}

int main(void) {
    Uint32 startTime = SDL_GetTicks();

    init(&resMgr.win, &resMgr.ren, &resMgr.font);
    setup();
    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            event.type == SDL_QUIT ? (quit = 1) : 0;
            switch (event.key.keysym.sym) {
            case 'a':
                currentDir = LEFT;
                break;
            case 's':
                currentDir = DOWN;
                break;
            case 'd':
                currentDir = RIGHT;
                break;
            case 'w':
                currentDir = UP;
                break;
            case ' ':
                if (gameState == PLAYING) {
                    gameState = PAUSED;
                } else if (gameState == DEFEAT) {
                    gameState = PLAYING;
                    setup();
                } else {
                    gameState = PLAYING;
                }
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

        if (gameState == PLAYING || gameState == PAUSED) {
            Uint32 currentTime = SDL_GetTicks();
            Uint32 deltaTime = currentTime - startTime;

            if (gameState == PLAYING) {
                elapsedTime += deltaTime;
                minutes = elapsedTime / 60000;
                seconds = (elapsedTime / 1000) % 60;
                update();
            }

            startTime = currentTime;
        }

        render(resMgr.ren);

        SDL_RenderPresent(resMgr.ren);
        SDL_Delay(1000 / FPS);
    }
    destroyResources(resMgr.win, resMgr.ren, resMgr.font);
    SDL_Quit();
    return EXIT_SUCCESS;
}
