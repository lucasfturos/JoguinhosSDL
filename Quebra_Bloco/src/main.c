#include "include/init_sdl.h"

Bar bar = {
    .x = WIDTH / 2.f - BAR_LEN / 2.f,
    .y = HEIGHT - BAR_THICCNESS - 20.f,
};

Coord circle = {
    .x = WIDTH / 2.f,
    .y = HEIGHT / 2.f,
    .speedX = 5,
    .speedY = 5,
    .radius = 10,
};

TargetNode *targetList = NULL;

int pause = 1;
int gameover = 0;
int started = 1;
int score = 0;
int quit = 0;

Uint32 lastTime = 0;
float deltaTime = 0.0f;
const Uint32 frameDelay = 1000 / FPS;

void earnPoints(int pointsEarn) { score += pointsEarn; }

int checkCollision(Coord circle, SDL_Rect rect) {
    float rectX = rect.x + rect.w / 2.0f;
    float rectY = rect.y + rect.h / 2.0f;

    return fabsf(circle.x - rectX) <= circle.radius + rect.w / 2.0f &&
           fabsf(circle.y - rectY) <= circle.radius + rect.h / 2.0f;
}

void update(float deltaTime) {
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
    TargetNode *currentNode = targetList;
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

void render(Resources *res) {
    SDL_RenderClear(res->ren);
    SDL_RenderCopy(res->ren, res->bgTexture, NULL, NULL);

    drawCircle(res->ren, circle.x, circle.y, circle.radius,
               (SDL_Color){255, 0, 0, 255});

    SDL_Rect barRect =
        createRect(bar.x, bar.y - BAR_THICCNESS / 2.f, BAR_LEN, BAR_THICCNESS);
    SDL_RenderCopy(res->ren, res->barTexture, NULL, &barRect);

    TargetNode *cur = targetList;
    while (cur) {
        if (!cur->target.dead) {
            SDL_Rect r = createRect(cur->target.x, cur->target.y, TARGET_WIDTH,
                                    TARGET_HEIGHT);
            SDL_RenderCopy(res->ren, res->brickTexture, NULL, &r);
        }
        cur = cur->next;
    }

    drawScore(res->ren, res->font, score, res->bgScore);

    if (gameover)
        drawGameOver(res->ren, res->font);
}

void gameLoop(void *arg) {
    Resources *res = (Resources *)arg;
    Uint32 frameStart = SDL_GetTicks();

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            quit = 1;

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_a:
            case SDLK_LEFT:
                bar.x -= 10.f;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                bar.x += 10.f;
                break;
            case SDLK_SPACE:
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
            case SDLK_q:
                quit = 1;
                break;
            }
        }
    }

    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
    if (keyboard[SDL_SCANCODE_A] != 0 || keyboard[SDL_SCANCODE_LEFT] != 0) {
        bar.x -= 10.f;
        if (!started) {
            circle.x = -1;
            started = 0;
        }
    }
    if (keyboard[SDL_SCANCODE_D] != 0 || keyboard[SDL_SCANCODE_RIGHT] != 0) {
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
        drawGameOver(res->ren, res->font);
    } else {
        update(deltaTime);
        render(res);
        drawScore(res->ren, res->font, score, res->bgScore);
    }
    SDL_RenderPresent(res->ren);

#ifndef __EMSCRIPTEN__
    Uint32 frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < frameDelay)
        SDL_Delay(frameDelay - frameTime);
#endif
}

int main(void) {
    static Resources res;

    if (init(&res) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    targetList = initializeTargets();
    lastTime = SDL_GetTicks();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(gameLoop, &resMgr, 0, 1);
#else
    while (!quit)
        gameLoop(&res);
#endif

    destroyResources(&res, targetList);
    SDL_Quit();
    return EXIT_SUCCESS;
}
