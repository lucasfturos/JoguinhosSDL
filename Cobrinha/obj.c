#include "obj.h"

SDL_Rect createRect(int x, int y, int w, int h) {
    SDL_Rect rect = {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
    };
    return rect;
}

void drawApple(SDL_Renderer *ren, int x, int y) {
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_Rect appleRect = createRect(x, y, SIZE_APPLE, SIZE_APPLE);
    SDL_RenderFillRect(ren, &appleRect);
}

void drawSnake(SDL_Renderer *ren, int x, int y) {
    SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
    SDL_Rect snakeRect = createRect(x, y, SIZE_SNAKE, SIZE_SNAKE);
    SDL_RenderFillRect(ren, &snakeRect);
}

void drawText(SDL_Renderer *ren, TTF_Font *font, int boardx, int score,
              int level) {
    SDL_Color textColor = {255, 255, 255, 255};

    // Score
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
    SDL_Surface *scoreSurface =
        TTF_RenderText_Solid(font, scoreText, textColor);
    SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(ren, scoreSurface);
    SDL_Rect scoreRect = {
        .x = boardx,
        .y = 10,
        .w = scoreSurface->w,
        .h = scoreSurface->h,
    };
    SDL_RenderCopy(ren, scoreTexture, NULL, &scoreRect);

    // Level
    char levelText[50];
    snprintf(levelText, sizeof(scoreText), "Level: %d", level);
    SDL_Surface *levelSurface =
        TTF_RenderText_Solid(font, levelText, textColor);
    SDL_Texture *levelTexture = SDL_CreateTextureFromSurface(ren, levelSurface);
    int levelTextWidth, levelTextHeight;
    TTF_SizeText(font, levelText, &levelTextWidth, &levelTextHeight);
    SDL_Rect levelRect = {
        .x = BOARD_W - levelTextWidth + 40,
        .y = 10,
        .w = levelTextWidth,
        .h = levelTextHeight,
    };
    SDL_RenderCopy(ren, levelTexture, NULL, &levelRect);

    SDL_FreeSurface(scoreSurface);
    SDL_FreeSurface(levelSurface);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(levelTexture);
}

void drawBoard(SDL_Renderer *ren, int boardx, int boardy, int borderThicness) {

    SDL_Rect top = createRect(boardx, boardy, BOARD_W, borderThicness);
    SDL_Rect bottom = createRect(boardx, boardy + BOARD_H - borderThicness,
                                 BOARD_W, borderThicness);
    SDL_Rect left = createRect(boardx, boardy, borderThicness, BOARD_H);
    SDL_Rect right = createRect(boardx + BOARD_W - borderThicness, boardy,
                                borderThicness, BOARD_H);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderFillRect(ren, &top);
    SDL_RenderFillRect(ren, &bottom);
    SDL_RenderFillRect(ren, &left);
    SDL_RenderFillRect(ren, &right);
}
