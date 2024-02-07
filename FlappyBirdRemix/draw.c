#include "draw.h"

void drawPause(Resources *res) {
    SDL_Color outlineColor = {0, 0, 0, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    char *pauseText = "Paused!";

    int textWidth, textHeight;
    TTF_SizeText(res->font, pauseText, &textWidth, &textHeight);

    int x = (WIDTH - textWidth) / 2;
    int y = (HEIGHT - textHeight) / 2;

    drawTextWithOutline(res->ren, res->font, x, y, pauseText, textColor,
                        outlineColor, 3);
}

void drawGameover(Resources *res) {
    SDL_Color outlineColor = {0, 0, 0, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    char *gameoverText = "Press SPACE to Restart!";

    int textWidth, textHeight;
    TTF_SizeText(res->font, gameoverText, &textWidth, &textHeight);

    int x = (WIDTH - textWidth) / 2;
    int y = (HEIGHT - textHeight) / 2;

    drawTextWithOutline(res->ren, res->font, x, y, gameoverText, textColor,
                        outlineColor, 3);
}

void drawScore(Resources *res, int score) {
    char scoreText[100];
    snprintf(scoreText, sizeof(scoreText), "%d", score);
    SDL_Color outlineColor = {0, 0, 0, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    drawTextWithOutline(res->ren, res->font, 10, 10, scoreText, textColor,
                        outlineColor, 3);
}

void drawPipe(Resources *res, SDL_Point point, int pipeRotation) {
    SDL_Rect pipeSrcRect = {
        .x = 0,
        .y = 0,
        .w = PIPE_WIDTH,
        .h = PIPE_HEIGHT,
    };

    SDL_Rect pipeDestRect = {
        .x = point.x,
        .y = point.y,
        .w = PIPE_WIDTH * 1.5,
        .h = PIPE_HEIGHT * 1.5,
    };

    SDL_RenderCopyEx(res->ren, res->pipeTex, &pipeSrcRect, &pipeDestRect,
                     pipeRotation, NULL, SDL_FLIP_NONE);
}

void drawBird(Resources *res, SDL_Point point, float birdAngle) {
    SDL_Rect birdSrcRect = {
        .x = point.x,
        .y = 0,
        .w = BIRD_WIDTH,
        .h = BIRD_HEIGHT,
    };
    SDL_Rect birdDestRect = {
        .x = WIDTH / 2 - BIRD_WIDTH,
        .y = (HEIGHT / 2.f - BIRD_HEIGHT) + point.y,
        .w = BIRD_WIDTH * 1.6,
        .h = BIRD_HEIGHT * 1.6,
    };

    SDL_Point center = {
        .x = BIRD_WIDTH / 2,
        .y = BIRD_HEIGHT / 2,
    };

    SDL_RenderCopyEx(res->ren, res->birdTex, &birdSrcRect, &birdDestRect,
                     birdAngle, &center, SDL_FLIP_NONE);
}

void drawTextWithOutline(SDL_Renderer *ren, TTF_Font *font, int x, int y,
                         char *text, SDL_Color textColor,
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
                    .w = textWidth,
                    .h = textHeight,
                };
                SDL_RenderCopy(ren, outlineTexture, NULL, &destRect);
            }
        }
    }

    SDL_Rect textRect = {
        .x = x,
        .y = y,
        .w = textWidth,
        .h = textHeight,
    };
    SDL_RenderCopy(ren, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(outlineSurface);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(outlineTexture);
}
