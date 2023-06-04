#include <iostream>
#include <SDL2/SDL.h>
#include "nutils.h"
#include "movement.h"
#include "sdl_boilerplate.h"

void nStartUp()
{

    // Initialize SDL2 and create a window and a renderer
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_GAMECONTROLLER);
    window = SDL_CreateWindow("Minecraft 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    //SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | (SDL_RENDERER_PRESENTVSYNC * vsync));

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

void nMouseTick()
{
    SDL_GetGlobalMouseState(&xMouse,&yMouse);
    int windowX, windowY;
    SDL_GetWindowPosition(window, &windowX, &windowY);
    xMouse -= windowX;
    yMouse -= windowY;

    Uint32 mouseState = SDL_GetMouseState(&xMouse, &yMouse);
    leftMouse = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    rightMouse = (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
}

int nmod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}
