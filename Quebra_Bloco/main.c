#include "objects.h"

ResMgr resMgr;

Bar bar = {.x = WIDTH / 2.f - BAR_LEN / 2.f,
           .y = HEIGHT - BAR_THICCNESS - 20.f};

Coord circle = {.x = WIDTH / 2.f,
                .y = HEIGHT / 2.f,
                .speedX = 5,
                .speedY = 5,
                .radius = 10};

int pause = 1;
int gameover = 0;
int started = 1;
int score = 0;

void earnPoints(int pointsEarn) { score += pointsEarn; }

int checkCollision(Coord circle, SDL_Rect rect) {
    float circleX = circle.x;
    float circleY = circle.y;

    float rectX = rect.x + rect.w / 2.0;
    float rectY = rect.y + rect.h / 2.0;

    float deltaX = circleX - rectX;
    float deltaY = circleY - rectY;

    float maxDistX = circle.radius + rect.w / 2.0;
    float maxDistY = circle.radius + rect.h / 2.0;

    if (fabsf(deltaX) <= maxDistX && fabsf(deltaY) <= maxDistY) {
        return 1;
    }

    return 0;
}

void render(SDL_Renderer *ren, SDL_Texture *brickTexture,
            SDL_Texture *barTexture, SDL_Texture *bgTexture,
            TargetNode *targetList) {
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, bgTexture, NULL, NULL);

    SDL_Color color_circle = {255, 0, 0, 255};
    drawCircle(ren, circle.x, circle.y, circle.radius, color_circle);

    SDL_Rect barRect = createRect(
        (int)bar.x, (int)(bar.y - BAR_THICCNESS / 2.f), BAR_LEN, BAR_THICCNESS);
    SDL_RenderCopy(ren, barTexture, NULL, &barRect);

    TargetNode *currentNode = targetList;
    while (currentNode != NULL) {
        if (!currentNode->target.dead) {
            SDL_Rect targetRect = createRect((int)currentNode->target.x,
                                             (int)currentNode->target.y,
                                             TARGET_WIDTH, TARGET_HEIGHT);
            SDL_SetTextureColorMod(brickTexture, 255, 0, 0);
            SDL_RenderCopy(ren, brickTexture, NULL, &targetRect);
        }
        currentNode = currentNode->next;
    }
}

void update(TargetNode **targetList, float deltaTime) {
    if (!pause && started) {
        circle.x += circle.speedX * deltaTime;
        circle.y += circle.speedY * deltaTime;

        if (circle.x < circle.radius || circle.x > WIDTH - circle.radius) {
            circle.speedX = -circle.speedX;
        }
        if (circle.y - TARGET_START_Y < circle.radius ||
            circle.y > HEIGHT - circle.radius) {
            circle.speedY = -circle.speedY;
        }

        if (circle.y + circle.radius >= bar.y &&
            circle.y - circle.radius <= bar.y) {
            if (circle.x + circle.radius >= bar.x &&
                circle.x - circle.radius <= bar.x + BAR_LEN) {
                circle.speedY = -circle.speedY;

                if (circle.y > bar.y) {
                    circle.y = bar.y + circle.radius + BAR_THICCNESS / 2.f;
                } else {
                    circle.y = bar.y - circle.radius - BAR_THICCNESS / 2.f;
                }
            }
        }
        if (circle.y >= HEIGHT - circle.radius - 1) {
            gameover = 1;
        }
    }

    if (bar.x < 0) {
        bar.x = 0;
    }
    if (bar.x > WIDTH - BAR_LEN) {
        bar.x = WIDTH - BAR_LEN;
    }

    TargetNode *currentNode = *targetList;
    while (currentNode != NULL) {
        if (!currentNode->target.dead) {
            SDL_Rect targetRect = createRect((int)currentNode->target.x,
                                             (int)currentNode->target.y,
                                             TARGET_WIDTH, TARGET_HEIGHT);
            if (checkCollision(circle, targetRect)) {
                currentNode->target.dead = 1;
                circle.speedY = -circle.speedY;
                earnPoints(10);
            }
        }
        currentNode = currentNode->next;
    }
}
void destroyResources(TargetNode *targetList) {
    TTF_CloseFont(resMgr.font);
    SDL_DestroyWindow(resMgr.win);
    SDL_DestroyRenderer(resMgr.ren);
    destroyTargets(targetList);
    SDL_DestroyTexture(resMgr.barTexture);
    SDL_DestroyTexture(resMgr.brickTexture);
    SDL_DestroyTexture(resMgr.bgScore);
    SDL_DestroyTexture(resMgr.bgTexture);
}

int main(void) {
    Uint32 startTime = SDL_GetTicks();
    Uint32 lastTime = startTime;
    float deltaTime = 0.0f;
    Uint32 frameDelay = 1000 / FPS;
    TargetNode *targetList = initializeTargets();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    resMgr.win = SDL_CreateWindow("Quebrando Bloquinho", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                                  SDL_WINDOW_VULKAN);
    if (resMgr.win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        destroyResources(targetList);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    resMgr.ren = SDL_CreateRenderer(
        resMgr.win, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    if (resMgr.ren == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        destroyResources(targetList);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "Erro ao inicializar SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    resMgr.barTexture = IMG_LoadTexture(resMgr.ren, "img/Bar.png");
    resMgr.brickTexture = IMG_LoadTexture(resMgr.ren, "img/Black_Brick.png");
    resMgr.bgScore = IMG_LoadTexture(resMgr.ren, "img/score_board.png");
    resMgr.bgTexture = IMG_LoadTexture(resMgr.ren, "img/background_sky.png");
    resMgr.font = TTF_OpenFont("font/times.ttf", 30);
    if (!resMgr.barTexture || !resMgr.brickTexture || !resMgr.bgScore ||
        !resMgr.bgTexture || !resMgr.font) {
        fprintf(stderr, "Erro ao carregar recursos: %s\n", SDL_GetError());
        destroyResources(targetList);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    int quit = 0;
    while (!quit) {
        Uint32 frameStart = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            switch (event.key.keysym.sym) {
            case 'a':
                bar.x -= 10.f;
                break;
            case 'd':
                bar.x += 10.f;
                break;
            case ' ':
                if (gameover) {
                    gameover = 0;
                    started = 1;
                    score = 0;
                    circle.x = WIDTH / 2.f;
                    circle.y = HEIGHT / 2.f;
                    circle.speedX = 5;
                    circle.speedY = 5;
                    bar.x = WIDTH / 2.f - BAR_LEN / 2.f;
                    targetList = initializeTargets();
                } else {
                    pause = !pause;
                }
                break;
            case 'q':
                quit = 1;
                break;
            }
        }

        const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
        if (keyboard[SDL_SCANCODE_A] != 0) {
            bar.x -= 10.f;
            if (!started) {
                circle.x = -1;
                started = 0;
            }
        }
        if (keyboard[SDL_SCANCODE_D] != 0) {
            bar.x += 10.f;
            if (!started) {
                circle.y = -1;
                started = 0;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 15.0f;
        lastTime = currentTime;

        if (gameover) {
            drawGameOver(resMgr.ren, resMgr.font);
        } else {
            update(&targetList, deltaTime);
            render(resMgr.ren, resMgr.brickTexture, resMgr.barTexture,
                   resMgr.bgTexture, targetList);
            drawScore(resMgr.ren, resMgr.font, score, resMgr.bgScore);
        }
        SDL_RenderPresent(resMgr.ren);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    destroyResources(targetList);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
