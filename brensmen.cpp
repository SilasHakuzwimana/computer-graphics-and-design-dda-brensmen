#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

const int WIDTH = 800;
const int HEIGHT = 600;
const int CENTER_X = WIDTH / 2;
const int CENTER_Y = HEIGHT / 2;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init Failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("Bresenham Line Drawing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void drawAxes() {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawLine(renderer, CENTER_X, 0, CENTER_X, HEIGHT);
    SDL_RenderDrawLine(renderer, 0, CENTER_Y, WIDTH, CENTER_Y);
}

void plotPoint(int x, int y) {
    SDL_RenderDrawPoint(renderer, CENTER_X + x, CENTER_Y - y);
}

void drawBresenham(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        plotPoint(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

int applyQuadrant(int quadrant, int x, int y) {
    switch (quadrant) {
        case 1: return x;         // +x, +y
        case 2: return -x;        // -x, +y
        case 3: return -x;        // -x, -y
        case 4: return x;         // +x, -y
        default: return x;
    }
}

int applyQuadrantY(int quadrant, int x, int y) {
    switch (quadrant) {
        case 1: return y;
        case 2: return y;
        case 3: return -y;
        case 4: return -y;
        default: return y;
    }
}

int main() {
    initSDL();
    drawAxes();

    int quadrant;
    std::cout << "Choose quadrant to draw in (1 to 4): ";
    std::cin >> quadrant;

    int x1, y1, x2, y2;
    std::cout << "Enter x1 y1: ";
    std::cin >> x1 >> y1;
    std::cout << "Enter x2 y2: ";
    std::cin >> x2 >> y2;

    // Apply quadrant transformation
    x1 = applyQuadrant(quadrant, x1, y1);
    y1 = applyQuadrantY(quadrant, x1, y1);
    x2 = applyQuadrant(quadrant, x2, y2);
    y2 = applyQuadrantY(quadrant, x2, y2);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
    drawBresenham(x1, y1, x2, y2);
    SDL_RenderPresent(renderer);

    std::cout << "Line drawn using Bresenham in quadrant " << quadrant << ". Close window to exit.\n";

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
