#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>
#include "tick.h"
#include "nutils.h"
#include "world.h"
#include "blockproperties.h"
#include "movement.h"

SDL_Window *window;
int windowWidth, windowHeight;
SDL_Renderer *renderer;

float camx = 0;
float camy = 4096;

SDL_Texture *textures[4];
SDL_Texture *entityTextures[1];

int8_t selectorMode = 0;
int8_t oldSelectorMode = 0;

Uint32 lastFrameTime;
int frameCount = 0;
Uint32 last_time;
Uint32 current_time;

SDL_GameController *gameController;
const Uint8 *keyboard_state = nullptr;

int joyxa;
int joyya;
int joyxb;
int joyyb;

void renderBlocks()
{
    int16_t tile;
    SDL_Texture *selected_texture;
    SDL_Rect dest_rect;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    for (int i = (windowWidth / -128); i < ((windowWidth / 64) + 3); i++) {
        for (int j = (windowHeight / -128); j < (windowHeight / 64) + 3; j++) {
            dest_rect = {
                            (windowWidth / 2) + (64 * i) - nmod(camx, 64) - 64,
                            (windowHeight / 2) + (-64 * j) + nmod(camy, 64),
                            64, 64
                        };
            tile = getTile(camx + (i * 64) - 64, camy + (j * 64));
            if (tile > 0) {
                SDL_RenderCopy(renderer, textures[tile], NULL, &dest_rect);
            }
        }
    }
}

void tickEntities()
{
    for (int e = 0; e < 1024; e++) {
        if (entities[e] == nullptr) {
            break;
        }
        entities[e]->tick();
    }
}

void tickSelector()
{
    int selectedBlockY = std::max((int)floor((nmod(windowHeight / 2, 64) - yMouse + camy) / 64) + (windowHeight / 128) + 1, 0);
    int selectedBlockX = std::max((int)floor((xMouse - nmod(windowWidth / 2, 64) + camx) / 64) - (windowWidth / 128), 0);
    int selectedX = (64 * selectedBlockX) + (windowWidth / 2) - camx;
    int selectedY = (windowHeight / 2) - ((64 * selectedBlockY) - camy);

    if (leftMouse ||
     SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A))
    {
        int16_t currentBlock = world[selectedBlockY][selectedBlockX];
        if (selectorMode == 0) {
            if (currentBlock == 0) {
                selectorMode = -1;
            } else {
                selectorMode = 1;
            }
        }
        if (selectorMode == 1) {
            world[selectedBlockY][selectedBlockX] = 0;
        } else if (!(blockProperties[world[selectedBlockY][selectedBlockX]].isBlock)) {
            world[selectedBlockY][selectedBlockX] = 1;
        }
    } else {
        selectorMode = 0;
    }

    SDL_Rect dest_rect = {
        selectedX,
        selectedY,
        65, 64};
    SDL_RenderCopyEx(renderer, textures[0], NULL, &dest_rect, 0, NULL, SDL_FLIP_NONE);
}

void tickMovementAndFPS()
{
    // Calculate FPS
    float currentFrameTime = SDL_GetTicks();
    float elapsedFrameTime = currentFrameTime - lastFrameTime;

    if (elapsedFrameTime >= 1000) {
        // One second has elapsed, so calculate FPS
        fps = frameCount / (elapsedFrameTime / 1000.0f);

        // Print the FPS value to the console
        // if(vsync == 0) { std::cout << "FPS: " << fps << "\n"; }

        // Reset the frame count and last frame time
        frameCount = 0;
        lastFrameTime = currentFrameTime;
    }
    frameCount++;

    current_time = SDL_GetTicks();
    delta = (current_time - last_time) / 1;
    last_time = current_time;

    if (keyboard_state[SDL_SCANCODE_D]) {
        entities[0]->speedX += xspeed * delta;
    }
    if (keyboard_state[SDL_SCANCODE_A]) {
        entities[0]->speedX -= xspeed * delta;
    }
    joyxa = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX);
    joyya = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY);

    camx += (entities[0]->x - camx) * delta / 128;
    if(camx < (windowWidth / 2)) { camx = (windowWidth / 2); }
    camy += (entities[0]->y - camy) * delta / 128;
}
