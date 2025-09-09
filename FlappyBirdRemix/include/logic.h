#ifndef LOGIC_H
#define LOGIC_H

#include "flappy.h"

const int pipeGap = 120;
const int initialPipeCount = 4;
const int minPipeDistance = 200;
const int maxPipeDistance = 400;

extern int windowWidth;
extern int windowHeight;
extern float scaleX;
extern float scaleY;

static void checkCollision(SDL_Point *birdPoint, Pipe *head,
                           enum GameState *state, int *score,
                           float *birdVelocity) {
    int scaledBirdW = (int)(BIRD_WIDTH * 1.6f * scaleX);
    int scaledBirdH = (int)(BIRD_HEIGHT * 1.6f * scaleY);
    SDL_Rect birdRect = {
        .x = (int)(windowWidth / 2 - scaledBirdW / 2),
        .y = (int)((windowHeight / 2.0f - scaledBirdH / 2.0f) +
                   birdPoint->y * scaleY),
        .w = scaledBirdW,
        .h = scaledBirdH,
    };

    int scaledPipeW = (int)(PIPE_WIDTH * 1.5f * scaleX);
    int scaledPipeH = (int)(PIPE_HEIGHT * 1.5f * scaleY);

    Pipe *current = head;
    while (current != NULL) {
        SDL_Rect topPipe = {(int)(current->x * scaleX),
                            (int)(current->topY * scaleY), scaledPipeW,
                            scaledPipeH};
        SDL_Rect bottomPipe = {(int)(current->x * scaleX),
                               (int)(current->bottomY * scaleY), scaledPipeW,
                               scaledPipeH};

        if (SDL_HasIntersection(&birdRect, &topPipe) ||
            SDL_HasIntersection(&birdRect, &bottomPipe)) {
            *state = DEFEAT;
            *birdVelocity = 2.5f;
            return;
        }

        int birdCenterX = windowWidth / 2;
        if (!current->passed &&
            current->x * scaleX + scaledPipeW < birdCenterX) {
            current->passed = 1;
            (*score)++;
        }

        current = current->next;
    }

    if (birdRect.y <= 0 || birdRect.y + birdRect.h >= windowHeight) {
        *state = DEFEAT;
    }
}

static void createPipe(Pipe **head) {
    Pipe *newPipe = (Pipe *)malloc(sizeof(Pipe));
    if (!newPipe) {
        printf("Erro ao alocar memória para o novo tubo.\n");
        exit(EXIT_FAILURE);
    }

    int randomY = rand() % 275 + 175;
    newPipe->x = WIDTH - PIPE_WIDTH;
    newPipe->topY = randomY - pipeGap - PIPE_HEIGHT;
    newPipe->bottomY = randomY + pipeGap;
    newPipe->passed = 0;
    newPipe->next = NULL;

    if (*head == NULL) {
        *head = newPipe;
    } else {
        Pipe *current = *head;
        while (current->next != NULL)
            current = current->next;
        current->next = newPipe;
    }
}

static void repositionPipe(Pipe *pipe, int score, int referenceX) {
    int minDistanceAdj = minPipeDistance - score / 10 * 2;
    int maxDistanceAdj = maxPipeDistance - score / 10 * 2;

    if (minDistanceAdj < 150)
        minDistanceAdj = 150;
    if (maxDistanceAdj < 200)
        maxDistanceAdj = 200;

    int spacing =
        minDistanceAdj + rand() % (maxDistanceAdj - minDistanceAdj + 1);
    pipe->x = referenceX + spacing;

    int randomY = rand() % 275 + 175;
    pipe->topY = randomY - pipeGap - PIPE_HEIGHT;
    pipe->bottomY = randomY + pipeGap;
    pipe->passed = 0;
}

static void initPipes(Pipe **head, int *score) {
    Pipe *lastPipe = NULL;
    int maxX = WIDTH;

    for (int i = 0; i < initialPipeCount; i++) {
        createPipe(head);
        Pipe *current = *head;
        while (current->next != NULL)
            current = current->next;

        if (lastPipe) {
            repositionPipe(current, *score, maxX);
        } else {
            current->x = maxX + minPipeDistance +
                         rand() % (maxPipeDistance - minPipeDistance + 1);
        }

        lastPipe = current;
        maxX = current->x;
    }
}

static void destroyPipes(Pipe **head) {
    Pipe *current = *head;
    while (current != NULL) {
        Pipe *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

#endif // LOGIC_H
