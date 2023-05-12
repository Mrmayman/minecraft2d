// external libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "FastNoiseLite.h"

// internal C libraries
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

//#ifdef _WIN32
//#include <direct.h>
//#define GetCurrentDir _getcwd
//#else
//#include <unistd.h>
//#define GetCurrentDir getcwd
//#endif

// internal C++ libraries
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>

// SDL initialization
SDL_Event event;
SDL_Renderer *renderer;
SDL_Rect dest_rect;
SDL_Window *window;
int windowWidth, windowHeight;
SDL_Surface *image;

// Texture initialization
SDL_Texture *textures[3];

std::string GamePath;
float camx = 0;
float camy = 4096;

int16_t world[128][4096];

SDL_Texture* nLoadTexture(std::string Path);
void generateWorld();
void renderBlocks();
int16_t getTile(float getTileX, float getTileY);
int nmod(int n, int m);

FastNoiseLite noise;

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    
    // Initialize SDL2 and create a window and a renderer
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Minecraft 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load the image file as a texture
    char *buf = SDL_GetBasePath();
    GamePath = buf;
    GamePath.append("Assets/");
    SDL_free(buf);
    
    textures[1] = nLoadTexture("wool.png");
    textures[2] = nLoadTexture("grass.png");
    
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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void generateWorld()
{
    int16_t block = 0;
    for(int j = 0; j < 4096; j++) {
        //world[i][j] = block;
        //srand(clock());
        //block = fmod(rand(), 3);
        block = (int16_t) (64 + (noise.GetNoise((float) j*2, 1.0f) * 10.0));
        block += (int16_t) (noise.GetNoise((float) j*5, 1.0f) * 3.0);
        for(int i = block; i > -1; i--) {
            world[i][j] = 2;
        }
        for(int i = block+1; i < 128; i++) {
            world[i][j] = 0;
        }
    }
}

void renderBlocks()
{
    int16_t tile;
    SDL_Texture *selected_texture;
    
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    for(int i = (windowWidth / -128); i < ((windowWidth / 64) + 3); i++) {
        for(int j = (windowHeight / -128); j < (windowHeight / 64) + 3; j++) {
            SDL_Rect dest_rect = { (windowWidth / 2) + (64 * i) - nmod(camx, 64) - 64,
                                   (windowHeight / 2) + (-64 * j) + nmod(camy, 64),
                                    64, 64 };
            tile = getTile( camx + (i * 64), camy + (j * 64) );
            if(tile > 0) {
                SDL_RenderCopy(renderer, textures[tile], NULL, &dest_rect);
            }
        }
    }
}

int16_t getTile(float getTileX, float getTileY)
{
    if(getTileX < 1) { return 0; }
    if(getTileY < 1) { return 0; }
    if(getTileY > (128 * 64)) { 0; }
    return world[(int) getTileY / 64][(int) getTileX / 64];
}

int nmod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

SDL_Texture* nLoadTexture(std::string Path)
{
    std::string TempPath = GamePath;
    image = IMG_Load(TempPath.append(Path).c_str());
    printf("Loaded texture: %s\n",TempPath.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return texture;
}
