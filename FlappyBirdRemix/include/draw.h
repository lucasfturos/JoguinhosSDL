#ifndef DRAW_H
#define DRAW_H

#include "flappy.h"

extern int windowWidth;
extern int windowHeight;
extern float scaleX;
extern float scaleY;

static void drawTextWithOutline(SDL_Renderer *ren, TTF_Font *font, int x, int y,
                                const char *text, SDL_Color textColor,
                                SDL_Color outlineColor, int outlineWidth) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(ren, textSurface);

    SDL_Surface *outlineSurface =
        TTF_RenderText_Solid(font, text, outlineColor);
    SDL_Texture *outlineTexture =
        SDL_CreateTextureFromSurface(ren, outlineSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    for (int dy = -outlineWidth; dy <= outlineWidth; ++dy) {
        for (int dx = -outlineWidth; dx <= outlineWidth; ++dx) {
            if (dx != 0 || dy != 0) {
                SDL_Rect destRect = {
                    .x = x + dx,
                    .y = y + dy,
                    .w = (int)(textWidth * scaleX),
                    .h = (int)(textHeight * scaleY),
                };
                SDL_RenderCopy(ren, outlineTexture, NULL, &destRect);
            }
        }
    }

    SDL_Rect textRect = {
        .x = x,
        .y = y,
        .w = (int)(textWidth * scaleX),
        .h = (int)(textHeight * scaleY),
    };
    SDL_RenderCopy(ren, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(outlineSurface);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(outlineTexture);
}

static void drawPause(Resources *res) {
    SDL_Color outlineColor = {0, 0, 0, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    const char *pauseText = "Paused!";

    int textWidth, textHeight;
    TTF_SizeText(res->font, pauseText, &textWidth, &textHeight);

    int x = (windowWidth - (int)(textWidth * scaleX)) / 2;
    int y = (windowHeight - (int)(textHeight * scaleY)) / 2;

    drawTextWithOutline(res->ren, res->font, x, y, pauseText, textColor,
                        outlineColor, 3);
}

static void drawGameover(Resources *res) {
    SDL_Color outlineColor = {0, 0, 0, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    const char *gameoverText = "Press SPACE to Restart!";

    int textWidth, textHeight;
    TTF_SizeText(res->font, gameoverText, &textWidth, &textHeight);

    int x = (windowWidth - (int)(textWidth * scaleX)) / 2;
    int y = (windowHeight - (int)(textHeight * scaleY)) / 2;

    drawTextWithOutline(res->ren, res->font, x, y, gameoverText, textColor,
                        outlineColor, 3);
}

static void drawScore(Resources *res, int score) {
    char scoreText[100];
    snprintf(scoreText, sizeof(scoreText), "%d", score);
    SDL_Color outlineColor = {0, 0, 0, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    drawTextWithOutline(res->ren, res->font, 10, 10, scoreText, textColor,
                        outlineColor, 3);
}

static void drawBird(Resources *res, SDL_Point point, float birdAngle) {
    SDL_Rect birdSrcRect = {0, 0, BIRD_WIDTH, BIRD_HEIGHT};

    SDL_Rect birdDestRect = {
        .x = (int)(windowWidth / 2.f - BIRD_WIDTH * 1.6f * scaleX / 2),
        .y = (int)((windowHeight / 2.f - BIRD_HEIGHT * 1.6f * scaleY / 2) +
                   point.y * scaleY),
        .w = (int)(BIRD_WIDTH * 1.6f * scaleX),
        .h = (int)(BIRD_HEIGHT * 1.6f * scaleY),
    };

    SDL_Point center = {
        (int)(BIRD_WIDTH / 2.f * scaleX),
        (int)(BIRD_HEIGHT / 2.f * scaleY),
    };

    SDL_RenderCopyEx(res->ren, res->birdTex, &birdSrcRect, &birdDestRect,
                     birdAngle, &center, SDL_FLIP_NONE);
}

static void drawPipe(Resources *res, SDL_Point point, int pipeRotation) {
    SDL_Rect pipeSrcRect = {0, 0, PIPE_WIDTH, PIPE_HEIGHT};

    SDL_Rect pipeDestRect = {
        .x = (int)(point.x * scaleX),
        .y = (int)(point.y * scaleY),
        .w = (int)(PIPE_WIDTH * 1.5f * scaleX),
        .h = (int)(PIPE_HEIGHT * 1.5f * scaleY),
    };

    SDL_RenderCopyEx(res->ren, res->pipeTex, &pipeSrcRect, &pipeDestRect,
                     pipeRotation, NULL, SDL_FLIP_NONE);
}

#endif // DRAW_H
