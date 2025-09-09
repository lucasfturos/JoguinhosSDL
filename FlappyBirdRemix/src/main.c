#include "include/draw.h"
#include "include/init_sdl.h"
#include "include/logic.h"

int windowWidth = WIDTH;
int windowHeight = HEIGHT;
float scaleX = 1.0f;
float scaleY = 1.0f;

SDL_Point birdPoint;
float gravity = 0.2f;
float birdVelocity = 0.f;
float jumpForce = -2.0f;
float birdAngle = 0.0f;

int currentFrame = 0;
int score = 0;

Pipe *head = NULL;
enum GameState gameState = PLAYING;

void update() {
    if (gameState != PLAYING) {
        birdPoint.y += birdVelocity;
        birdVelocity += gravity;
        if (birdPoint.y >=
            (windowHeight - (int)(BIRD_HEIGHT * 1.6f * scaleY))) {
            birdPoint.y = windowHeight - (int)(BIRD_HEIGHT * 1.6f * scaleY);
            birdVelocity = 0;
        }
        return;
    }

    birdPoint.y += birdVelocity;
    birdVelocity += gravity;
    birdAngle = birdVelocity > 0 ? 15.0f : birdVelocity < 0 ? -15.0f : 0.0f;

    Uint32 currentTime = SDL_GetTicks();
    currentFrame = (currentTime / 200) % NUM_FRAMES;
    birdPoint.x = currentFrame * BIRD_WIDTH;

    Pipe *current = head;
    int maxX = WIDTH;

    while (current != NULL) {
        current->x -= PIPE_SPEED;

        if (current->x <= -PIPE_WIDTH) {
            repositionPipe(current, score, maxX);
        }

        if (current->x > maxX)
            maxX = current->x;

        current = current->next;
    }

    checkCollision(&birdPoint, head, &gameState, &score, &birdVelocity);
}

void render(Resources *res) {
    SDL_RenderCopy(res->ren, res->bgTex, NULL, NULL);
    drawScore(res, score);
    drawBird(res, birdPoint, birdAngle);

    Pipe *current = head;
    while (current != NULL) {
        SDL_Point pointPipeTop = {current->x, current->topY};
        drawPipe(res, pointPipeTop, -180);

        SDL_Point pointPipeBottom = {current->x, current->bottomY};
        drawPipe(res, pointPipeBottom, 0);

        current = current->next;
    }

    if (gameState == PAUSED)
        drawPause(res);
    if (gameState == DEFEAT)
        drawGameover(res);
}

int main(void) {
    Resources res;
    int isPaused = 0;
    const Uint32 frameDelay = 1000 / FPS;

    if (init(&res) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    initPipes(&head, &score);

    int quit = 0;
    while (!quit) {
        Uint32 frameStart = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = 1;
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;
                    scaleX = (float)windowWidth / WIDTH;
                    scaleY = (float)windowHeight / HEIGHT;
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = 1;
                    break;
                case SDLK_p:
                    isPaused = !isPaused;
                    gameState = isPaused ? PAUSED : PLAYING;
                    break;
                case SDLK_SPACE:
                    if (gameState == DEFEAT) {
                        birdPoint.y = 0;
                        score = 0;
                        birdVelocity = jumpForce;
                        destroyPipes(&head);
                        gameState = PLAYING;
                        initPipes(&head, &score);
                    } else if (gameState == PLAYING) {
                        birdVelocity = jumpForce;
                    }
                    break;
                }
                break;
            }
        }

        const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
        if (keyboard[SDL_SCANCODE_SPACE]) {
            if (jumpForce > -5.0f)
                jumpForce -= 0.1f;
            else
                jumpForce = -2.0f;
        } else if (jumpForce < -2.0f) {
            jumpForce = -2.0f;
        }

        SDL_SetRenderDrawColor(res.ren, 0, 0, 0, 255);
        SDL_RenderClear(res.ren);

        update();
        render(&res);

        SDL_RenderPresent(res.ren);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay)
            SDL_Delay(frameDelay - frameTime);
    }

    destroyPipes(&head);
    destroyResources(&res);
    SDL_Quit();
    return 0;
}
