#include "objects.h"

TargetNode *initializeTargets() {
    TargetNode *head = NULL;
    for (int row = 0; row < TARGET_ROWS; row++) {
        for (int col = 0; col < TARGET_COLS; col++) {
            TargetNode *newNode = (TargetNode *)malloc(sizeof(TargetNode));
            newNode->target.x =
                TARGET_START_X + col * (TARGET_WIDTH + TARGET_PADDING);
            newNode->target.y =
                TARGET_START_Y + row * (TARGET_HEIGHT + TARGET_PADDING);
            newNode->target.dead = 0;
            newNode->next = head;
            head = newNode;
        }
    }
    return head;
}

void destroyTargets(TargetNode *head) {
    while (head != NULL) {
        TargetNode *temp = head;
        head = head->next;
        free(temp);
    }
}

void drawGameOver(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    font = TTF_OpenFont("./font/times.ttf", 70);
    SDL_Color color_red = {255, 0, 0, 255};
    SDL_Color color_white = {255, 255, 255, 255};
    SDL_Surface *textSurface =
        TTF_RenderText_Solid(font, "Perdeu", color_red);
    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {.x = (WIDTH - textSurface->w) / 2,
                         .y = (HEIGHT - textSurface->h) / 2,
                         .w = textSurface->w,
                         .h = textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    font = TTF_OpenFont("./font/times.ttf", 30);
    SDL_Surface *continueSurface = TTF_RenderText_Solid(
        font, "Clique Espaco para reiniciar!", color_white);
    SDL_Rect continueRect = {.x = (WIDTH - continueSurface->w) / 2,
                             .y = (HEIGHT - continueSurface->h) / 2 + 60,
                             .w = continueSurface->w,
                             .h = continueSurface->h};
    SDL_Texture *continueTexture =
        SDL_CreateTextureFromSurface(renderer, continueSurface);
    SDL_RenderCopy(renderer, continueTexture, NULL, &continueRect);
    SDL_RenderPresent(renderer);

    TTF_CloseFont(font);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(continueSurface);
    SDL_DestroyTexture(continueTexture);
}

void drawCircle(SDL_Renderer *renderer, float x, float y, int radius,
                SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            float dx = radius - w;
            float dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

SDL_Rect createRect(int x, int y, int w, int h) {
    SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};
    return rect;
}

void drawScore(SDL_Renderer *ren, TTF_Font *font, int score,
               SDL_Texture *bgScore) {
    SDL_Color textColor = {255, 255, 255, 255};
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(ren, textSurface);

    SDL_Rect bgRect = {.x = 10, .y = 5, .w = textSurface->w + 60, .h = 44};
    SDL_RenderCopy(ren, bgScore, NULL, &bgRect);

    SDL_Rect textRect = {
        .x = 20, .y = 10, .w = textSurface->w, .h = textSurface->h};
    SDL_RenderCopy(ren, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
