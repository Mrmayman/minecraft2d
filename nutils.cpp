#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "FastNoiseLite.h"
#include "globals.h"

void nStartUp(FastNoiseLite tempnoise)
{
    tempnoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    
    // Initialize SDL2 and create a window and a renderer
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_GAMECONTROLLER);
    window = SDL_CreateWindow("Minecraft 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    //SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load the image file as a texture
    char *buf = SDL_GetBasePath();
    GamePath = buf;
    GamePath.append("Assets/");
    SDL_free(buf);
    SDL_RenderPresent(renderer);
    running = true;
    keyboard_state = SDL_GetKeyboardState(NULL);
    
    if (SDL_NumJoysticks() >= 1) {
        gameController = SDL_GameControllerOpen(0);
        if (gameController == nullptr) {
        // Failed to open game controller
            std::cout << "No controllers detected\n";
        }
    }
}

SDL_Texture* nLoadTexture(std::string Path)
{
    std::string TempPath = GamePath;
    image = IMG_Load(TempPath.append(Path).c_str());
    //printf("Loaded texture: %s\n",TempPath.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return texture;
}

int nmod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}
