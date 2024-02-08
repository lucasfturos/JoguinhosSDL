#include "logic.h"

void createPipe(Pipe **head) {
    Pipe *newPipe = (Pipe *)malloc(sizeof(Pipe));
    if (newPipe == NULL) {
        printf("Erro ao alocar memória para o novo tubo.\n");
        exit(EXIT_FAILURE);
    }

    int randomY = rand() % 275 + 175;

    newPipe->x = WIDTH - PIPE_WIDTH;
    newPipe->topY = randomY - pipeGap - PIPE_HEIGHT;
    newPipe->bottomY = randomY + pipeGap;

    newPipe->next = NULL;

    if (*head == NULL) {
        *head = newPipe;
    } else {
        Pipe *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newPipe;
    }
}

void destroyPipes(Pipe **head) {
    Pipe *current = *head;
    while (current != NULL) {
        Pipe *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
}

void checkCollision() {}
