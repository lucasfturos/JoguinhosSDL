#ifndef LOGIC_H
#define LOGIC_H

#include "flappy.h"

static const int pipeGap = 120;

void checkCollision();
void createPipe(Pipe **head);
void destroyPipes(Pipe **head);

#endif// !LOGIC_H
