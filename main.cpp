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
bool running;
const Uint8 *keyboard_state;

// Texture initialization
SDL_Texture *textures[4];
SDL_Texture *entityTextures[1];

std::string GamePath;
float camx = 0;
float camy = 4096;
int seed = 0;

int16_t world[128][4096];

SDL_Texture* nLoadTexture(std::string Path);
void nStartUp(FastNoiseLite tempnoise);
void generateWorld(FastNoiseLite tempnoise);
void renderBlocks();
void tickEntities(Uint32 tempdelta);
void moveEntity(int tempentity, Uint32 tempdelta2);
int16_t getTile(float getTileX, float getTileY);
int nmod(int n, int m);

struct Entity {
    int id = 0;
    float x, y, speedX, speedY;
};
Entity entities[1024];

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    const float xspeed = 0.4;
    //std::cout << "Enter world seed:";
    //std::cin >> seed;
    seed = 1;
    FastNoiseLite noise(seed);
    nStartUp(noise);
    
    entityTextures[0] = nLoadTexture("player.png");
    textures[1] = nLoadTexture("grass.png");
    textures[2] = nLoadTexture("dirt.png");
    textures[3] = nLoadTexture("stone.png");
    
    generateWorld(noise);
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time, delta;    
    
    // spawn player
    entities[0].id = 1;
    entities[0].x = 62;
    entities[0].y = 69 * 64;
    
    while(running) {
        current_time = SDL_GetTicks();
        delta = current_time - last_time;
        last_time = current_time;

        if (keyboard_state[SDL_SCANCODE_D]) { entities[0].speedX += xspeed; }
        if (keyboard_state[SDL_SCANCODE_A]) { entities[0].speedX -= xspeed; }
        if (keyboard_state[SDL_SCANCODE_W] && getTile(entities[0].x, entities[0].y - 4) > 0) { entities[0].speedY += 2;entities[0].y += 4; }
        if (keyboard_state[SDL_SCANCODE_S]) { entities[0].speedY -= 1; }

        camx += (entities[0].x - camx) * delta/128;
        camy += (entities[0].y - camy) * delta/128;
        camx = std::max(camx, (float) (windowWidth / 2));
        camx = std::max(camx, (float) (windowWidth / 2));
        camy = std::max(camy, (float) (windowHeight / 2));
        camy = std::min(camy, (float) ((128*64) - (windowHeight*1.3)));

        SDL_SetRenderDrawColor(renderer, 180, 200, 255, 255);
        SDL_RenderClear(renderer);
        renderBlocks();
        tickEntities(delta);
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

void generateWorld(FastNoiseLite tempnoise)
{
    int16_t block = 0;
    for(int j = 0; j < 4096; j++) {
        block = (int16_t) (64 + (tempnoise.GetNoise((float) j*2, 1.0f) * 10.0));
        block += (int16_t) (tempnoise.GetNoise((float) j*5, 1.0f) * 3.0);
        for(int i = block; i > -1; i--) {
            world[i][j] = 3;
        }
        world[block+1][j] = 2;
        world[block+2][j] = 2;
        world[block+3][j] = 2;
        world[block+4][j] = 1;
        for(int i = block+5; i < 128; i++) {
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
            tile = getTile( camx + (i * 64) - 64, camy + (j * 64) );
            if(tile > 0) {
                SDL_RenderCopy(renderer, textures[tile], NULL, &dest_rect);
            }
        }
    }
}

int16_t getTile(float getTileX, float getTileY)
{
    if(getTileX < 0) { return 0; }
    if(getTileY < 0) { return 0; }
    if(getTileY > (128 * 64)) { 0; }
    return world[(int) floor(getTileY / 64)][(int) floor(getTileX / 64)];
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

void nStartUp(FastNoiseLite tempnoise)
{
    tempnoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    
    // Initialize SDL2 and create a window and a renderer
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Minecraft 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load the image file as a texture
    char *buf = SDL_GetBasePath();
    GamePath = buf;
    GamePath.append("Assets/");
    SDL_free(buf);
    SDL_RenderPresent(renderer);
    running = true;
    keyboard_state = SDL_GetKeyboardState(NULL);
}

void tickEntities(Uint32 tempdelta)
{
    for(int e = 0; e < 1024; e++) {
        if(entities[e].id == 0) {
            break;
        }
        moveEntity(e, tempdelta);
        entities[e].speedX *= 0.7;
        entities[e].speedY -= 0.001;
        entities[e].speedY *= 0.9;
        SDL_Rect dest_rect = { (int) ((windowWidth / 2) + entities[e].x - camx - 16), (int) ((windowHeight / 2) + camy - entities[e].y), 8*4, 32*4 };
        SDL_RenderCopy(renderer, entityTextures[entities[e].id - 1], NULL, &dest_rect);
    }
}

void moveEntity(int tempentity, Uint32 tempdelta2)
{
    for(int i = (int) abs(entities[tempentity].speedX); i > 0; i--) {
        entities[tempentity].x += tempdelta2 * (abs(entities[tempentity].speedX) / entities[tempentity].speedX);
        if(getTile(entities[tempentity].x, entities[tempentity].y) > 0) {
            i = 0;
            entities[tempentity].x -= tempdelta2 * (abs(entities[tempentity].speedX) / entities[tempentity].speedX);
            entities[tempentity].speedX = 0;
        }
    }
    for(float i = abs(entities[tempentity].speedY); i > 0; i--) {
        entities[tempentity].y += tempdelta2 * (abs(entities[tempentity].speedY) / entities[tempentity].speedY);
        if(getTile(entities[tempentity].x, entities[tempentity].y) > 0) {
            i = 0;
            entities[tempentity].y -= tempdelta2 * (abs(entities[tempentity].speedY) / entities[tempentity].speedY);
            entities[tempentity].speedY = 0;
        }
    }
}
