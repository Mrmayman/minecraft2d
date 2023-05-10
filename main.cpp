#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <cstdlib>

SDL_Event event;
SDL_Renderer *renderer;
SDL_Rect dest_rect;
SDL_Window *window;
int windowWidth, windowHeight;

SDL_Texture *texture;

float camx = 0;
float camy = 0;
int chunksum = 0;

int16_t world[128][4096];

void generateWorld();
void renderBlocks();
int16_t getTile(float getTileX, float getTileY);
int nmod(int n, int m);

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    // Initialize SDL2 and create a window and a renderer
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Minecraft 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load the image file as a texture
    SDL_Surface *image = IMG_Load("wool.png");
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    // Present the rendered frame on the screen
    SDL_RenderPresent(renderer);
    bool running = true;
    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
    generateWorld();
    
    (window, -1, SDL_RENDERER_ACCELERATED);

    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time, delta;    
    
    while(running) {
        current_time = SDL_GetTicks();
        delta = current_time - last_time;
        last_time = current_time;

        if (keyboard_state[SDL_SCANCODE_D]) { camx += delta; }
        if (keyboard_state[SDL_SCANCODE_A]) { camx -= delta; }
        if (keyboard_state[SDL_SCANCODE_W]) { camy += delta; }
        if (keyboard_state[SDL_SCANCODE_S]) { camy -= delta; }

        camx = round(camx);
        camy = round(camy);

        SDL_SetRenderDrawColor(renderer, 180, 200, 255, 255);
        SDL_RenderClear(renderer);
        renderBlocks();
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_RenderPresent(renderer);
    }

    // Destroy the texture, renderer, and window
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void generateWorld()
{
    int16_t block = 0;
    for(int i = 0; i < 128; i++) {
        for(int j = 0; j < 4096; j++) {
            world[i][j] = block;
            srand(clock());
            block = fmod(rand(), 2);
        }
    }
}

void renderBlocks()
{
    chunksum = 0;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    for(int i = (windowWidth / -128); i < ((windowWidth / 64) + 3); i++) {
        for(int j = (windowHeight / -128); j < (windowHeight / 64) + 3; j++) {
            SDL_Rect dest_rect = { (windowWidth / 2) + (64 * i) - nmod(camx, 64) - 64,
                                   (windowHeight / 2) + (-64 * j) + nmod(camy, 64),
                                    64, 64 };
            if(getTile( camx + (i * 64), 
                camy + (j * 64)
                      )) {
                SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
            }
        }
    }
    //printf("%d", chunksum);
}

int16_t getTile(float getTileX, float getTileY)
{
    if(getTileX < 1) { return world[-1][-1]; }
    if(getTileY < 1) { return world[-1][-1]; }
    if(getTileY > (128 * 64)) { return world[-1][-1]; }
    return world[(int) getTileY / 64][(int) getTileX / 64];
}

int nmod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}
