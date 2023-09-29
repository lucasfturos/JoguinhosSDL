#include "tetramino.h"

int init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    resMgr.win = SDL_CreateWindow("Tetramino", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                                  SDL_WINDOW_VULKAN);
    if (resMgr.win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        destroyResources();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    resMgr.ren = SDL_CreateRenderer(
        resMgr.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (resMgr.ren == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        destroyResources();
        SDL_Quit();
        return EXIT_FAILURE;
    }
    return 0;
}

void destroyResources() {
    SDL_DestroyRenderer(resMgr.ren);
    SDL_DestroyWindow(resMgr.win);
}

void render(SDL_Renderer *ren) {
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    drawPiece(ren);

    SDL_SetRenderDrawColor(ren, 61, 60, 58, 255);
    SDL_Rect interfaceRect = {.x = BOARD_W + SIZE_PIECE + 6,
                              .y = 0,
                              .w = WIDTH - BOARD_W,
                              .h = HEIGHT};
    SDL_RenderFillRect(ren, &interfaceRect);
}

void drawPiece(SDL_Renderer *ren) {
    SDL_SetRenderDrawColor(ren, red_piece, green_piece, blue_piece, 255);

    for (size_t square = 0; square < SQUARES; ++square) {
        z[square].x = forms[shape][square] % 2;
        z[square].y = forms[shape][square] / 2;

        SDL_Rect squareRect = {.x = pieceX + z[square].x * PADDING_PIECE,
                               .y = -SIZE_PIECE + pieceY +
                                    z[square].y * PADDING_PIECE,
                               .w = SIZE_PIECE,
                               .h = SIZE_PIECE};

        SDL_RenderFillRect(ren, &squareRect);
    }
}

void update() {
    rotate = 0;
    if (pieceX < 0) {
        pieceX = 0;
    }

    if (pieceX > BOARD_W - SIZE_PIECE) {
        pieceX = BOARD_W - SIZE_PIECE;
    }

    if (rotate) {
        Point coords = z[1];
        for (size_t square = 0; square < SQUARES; ++square) {
            int x = z[square].y - coords.y;
            int y = z[square].x - coords.x;
            z[square].x = coords.x - x;
            z[square].y = coords.y + y;
        }
    }

    for (size_t square = 0; square < SQUARES; ++square) {
        int y = z[square].y + pieceY + SIZE_PIECE * 2;
        if (y >= BOARD_H - SIZE_PIECE) {
            pieceY = 0;
            pieceX = BOARD_W / 2;
            shape = rand() % SHAPES;
            red_piece = rand() % 256;
            green_piece = rand() % 256;
            blue_piece = rand() % 256;
        }
    }
    pieceY += 5;
}

int main() {
    init();

    // Comando aleatório
    srand(time(0));
    shape = rand() % SHAPES;
    red_piece = rand() % 256;
    green_piece = rand() % 256;
    blue_piece = rand() % 256;

    // Loop principal
    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            event.type == SDL_QUIT ? (quit = 1) : 0;
            switch (event.key.keysym.sym) {
            case 'a':
                pieceX -= 5;
                break;
            case 'd':
                pieceX += 5;
                break;
            case 's':
                pieceY += 10;
                break;
            case 'w':
                rotate = 1;
                break;
            case 'q':
                quit = 1;
                break;
            }
        }

        const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
        if (keyboard[SDL_SCANCODE_A] != 0) {
            pieceX -= 5;
        }
        if (keyboard[SDL_SCANCODE_D] != 0) {
            pieceX += 5;
        }
        update();
        render(resMgr.ren);

        SDL_RenderPresent(resMgr.ren);
        SDL_Delay(1000 / FPS);
    }

    destroyResources();
    SDL_Quit();
    return EXIT_SUCCESS;
}
