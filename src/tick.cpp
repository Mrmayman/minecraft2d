#include <SDL2/SDL.h>
#include "tick.h"
#include "nutils.h"
#include "world.h"

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
Uint32 delta;
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
    for (int i = (windowWidth / -128); i < ((windowWidth / 64) + 3); i++)
    {
        for (int j = (windowHeight / -128); j < (windowHeight / 64) + 3; j++)
        {
            dest_rect = {(windowWidth / 2) + (64 * i) - nmod(camx, 64) - 64,
                         (windowHeight / 2) + (-64 * j) + nmod(camy, 64),
                         64, 64};
            tile = getTile(camx + (i * 64) - 64, camy + (j * 64));
            if (tile > 0)
            {
                SDL_RenderCopy(renderer, textures[tile], NULL, &dest_rect);
            }
        }
    }
}

void tickEntities()
{
    for (int e = 0; e < 1024; e++)
    {
        if (entities[e] == nullptr)
        {
            break;
        }
        entities[e]->move();
        entities[e]->speedX *= 1 - (delta * (1 - 0.98));
        entities[e]->speedY -= 0.01 * delta;
        if (entities[e]->speedX < 0)
        {
            entities[e]->direction = -1;
        }
        else if (entities[e]->speedX > 0)
        {
            entities[e]->direction = 1;
        }
        SDL_RendererFlip flip;
        if (entities[e]->direction < 0)
        {
            flip = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            flip = SDL_FLIP_NONE;
        }
        SDL_Rect dest_rect = {(int)((windowWidth / 2) + entities[e]->x - camx - 16), (int)((windowHeight / 2) + camy - entities[e]->y - 64), 8 * 4, 32 * 4};
        SDL_RenderCopyEx(renderer, entityTextures[entities[e]->id - 1], NULL, &dest_rect, 0, NULL, flip);
    }
}

void tickSelector()
{
    int selectedX = (64 * floor((xMouse + camx) / 64)) - camx + nmod(windowWidth / 2, 64);
    int selectedY = windowHeight -
                    ((64 * floor((windowHeight - yMouse + 16 + camy) / 64)) -
                     camy + nmod(windowHeight / 2, 64));

    if (leftMouse or ((bool)SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A)))
    {
        int selectedBlockY = std::max((int)floor((camy - selectedY) / 64) + (windowHeight / 128) + 1, 0);
        int selectedBlockX = std::max((int)floor((selectedX + camx) / 64) - (windowWidth / 128) + 1, 0);
        int16_t currentBlock = world[selectedBlockY][selectedBlockX];
        if (selectorMode == 0)
        {
            if (currentBlock == 0)
            {
                selectorMode = -1;
            }
            else
            {
                selectorMode = 1;
            }
        }
        if (selectorMode == 1)
        {
            world[selectedBlockY][selectedBlockX] = 0;
        }
        else
        {
            world[selectedBlockY][selectedBlockX] = 1;
        }
    }
    else
    {
        selectorMode = 0;
    }

    SDL_Rect dest_rect = {
        (int)(selectedX),
        (int)(selectedY),
        65, 64};
    SDL_RenderCopyEx(renderer, textures[0], NULL, &dest_rect, 0, NULL, SDL_FLIP_NONE);
}

void tickMovementAndFPS()
{
    // Calculate FPS
    Uint32 currentFrameTime = SDL_GetTicks();
    Uint32 elapsedFrameTime = currentFrameTime - lastFrameTime;

    if (elapsedFrameTime >= 1000)
    {
        // One second has elapsed, so calculate FPS
        float fps = frameCount / (elapsedFrameTime / 1000.0f);

        // Print the FPS value to the console
        // if(vsync == 0) { std::cout << "FPS: " << fps << "\n"; }

        // Reset the frame count and last frame time
        frameCount = 0;
        lastFrameTime = currentFrameTime;
    }
    frameCount++;

    current_time = SDL_GetTicks();
    delta = current_time - last_time;
    last_time = current_time;

    if (keyboard_state[SDL_SCANCODE_D])
    {
        entities[0]->speedX += xspeed * delta;
    }
    if (keyboard_state[SDL_SCANCODE_A])
    {
        entities[0]->speedX -= xspeed * delta;
    }
    joyxa = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX);
    joyya = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY);

    camx += (entities[0]->x - camx) * delta / 128;
    camy += (entities[0]->y - camy) * delta / 128;
}