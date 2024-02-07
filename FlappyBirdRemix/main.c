#include "draw.h"
#include "init_sdl.h"

SDL_Point birdPoint;
float gravity = 0.2f;
float birdVelocity = 0.f;
float jumpForce = -2.0f;
float birdAngle = 0.0f;

int currentFrame = 0;
const int pipeGap = 120;
int pipeTopY, pipeBottomY;
static int pipeX = WIDTH - PIPE_WIDTH;
int score = 0, count = 0;
enum GameState gameState = PLAYING;

void resetValue() {
    birdPoint.y = 0;
    score = 0;
    pipeX = WIDTH;
    birdVelocity = jumpForce;
}

void checkCollision() {}

void update() {
    if (gameState != PAUSED) {
        // Bird Update
        birdPoint.y += birdVelocity;
        birdVelocity += gravity;

        birdAngle = birdVelocity > 0 ? 15.0f : birdVelocity < 0 ? -15.0f : 0.0f;

        Uint32 currentTime = SDL_GetTicks();
        currentFrame = (currentTime / 200) % NUM_FRAMES;
        birdPoint.x = currentFrame * BIRD_WIDTH;

        if (birdPoint.y >= HEIGHT - BIRD_HEIGHT * 1.6) {
            gameState = DEFEAT;
        }

        // Pipe Update
        pipeX -= PIPE_SPEED;
        if (pipeX <= -PIPE_WIDTH) {
            pipeX = WIDTH;
        }

        pipeTopY = HEIGHT / 2 - pipeGap - PIPE_HEIGHT;
        pipeBottomY = HEIGHT / 2 + pipeGap;

        checkCollision();
    }
}

void render(Resources *res) {
    SDL_RenderCopy(res->ren, res->bgTex, NULL, NULL);
    drawScore(res, score);

    drawBird(res, birdPoint, birdAngle);

    SDL_Point pointPipeTop = {pipeX, pipeTopY};
    drawPipe(res, pointPipeTop, -180);

    SDL_Point pointPipeBottom = {pipeX, pipeBottomY};
    drawPipe(res, pointPipeBottom, 0);

    gameState == PAUSED ? drawPause(res) : 0;
    gameState == DEFEAT ? drawGameover(res) : 0;
}

int main(void) {
    Resources res;
    int isPaused = 0;
    const Uint32 frameDelay = 1000 / FPS;

    init(&res);

    int quit = 0;
    while (!quit) {
        Uint32 frameStart = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            event.type == SDL_QUIT ? (quit = 1) : 0;
            switch (event.key.keysym.sym) {
            case 'p':
                isPaused = !isPaused;
                gameState = isPaused ? PAUSED : PLAYING;
                break;
            case ' ':
                if (gameState == DEFEAT) {
                    resetValue();
                    gameState = PLAYING;
                }
                if (gameState == PLAYING) {
                    birdVelocity = jumpForce;
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
        if (keyboard[SDL_SCANCODE_SPACE] != 0) {
            if (jumpForce > -5.0f) {
                jumpForce -= 0.1f;
            } else {
                jumpForce = -2.0f;
            }
        } else {
            if (jumpForce < -2.0f) {
                jumpForce = -2.0f;
            }
        }

        SDL_SetRenderDrawColor(res.ren, 0, 0, 0, 255);
        SDL_RenderClear(res.ren);

        update();
        render(&res);

        SDL_RenderPresent(res.ren);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        (frameTime < frameDelay) ? SDL_Delay(frameDelay - frameTime) : 0;
    }
    destroyResources(&res);
    SDL_Quit();
    return 0;
}
