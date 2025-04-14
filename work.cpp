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
    window = SDL_CreateWindow("DDA & Circle Algorithms", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White bg
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

void drawDDA(int x1, int y1, int x2, int y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = std::max(abs(dx), abs(dy));
    float xInc = dx / steps;
    float yInc = dy / steps;
    float x = x1, y = y1;

    for (int i = 0; i <= steps; i++) {
        plotPoint(round(x), round(y));
        x += xInc;
        y += yInc;
    }
}

void drawCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 1 - r;
    while (x <= y) {
        plotPoint(xc + x, yc + y);
        plotPoint(xc - x, yc + y);
        plotPoint(xc + x, yc - y);
        plotPoint(xc - x, yc - y);
        plotPoint(xc + y, yc + x);
        plotPoint(xc - y, yc + x);
        plotPoint(xc + y, yc - x);
        plotPoint(xc - y, yc - x);
        x++;
        if (d < 0) d += 2 * x + 1;
        else {
            y--;
            d += 2 * (x - y) + 1;
        }
    }
}

int main() {
    initSDL();
    drawAxes();

    int choice;
    std::cout << "Choose what to draw:\n1. Line (DDA)\n2. Circle\nEnter choice: ";
    std::cin >> choice;

    int x1, y1, x2, y2, r;

    if (choice == 1) {
        std::cout << "Enter starting point (x1 y1): ";
        std::cin >> x1 >> y1;
        std::cout << "Enter ending point (x2 y2): ";
        std::cin >> x2 >> y2;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        drawDDA(x1, y1, x2, y2);

    } else if (choice == 2) {
        std::cout << "Enter center (x y): ";
        std::cin >> x1 >> y1;
        std::cout << "Enter radius: ";
        std::cin >> r;

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
        drawCircle(x1, y1, r);

    } else {
        std::cout << "Invalid choice." << std::endl;
    }

    SDL_RenderPresent(renderer);

    std::cout << "Drawing complete. Close the window to exit." << std::endl;

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
