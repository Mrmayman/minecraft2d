/*
    minecraft2d - A 2D Minecraft game in C++ and SDL2
    Copyright (C) 2023  Mrmayman

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Contact Mrmayman at navneetkrishna22@gmail.com
*/

// external libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "FastNoiseLite.h"

// headers
#include "entity.h"
#include "tile.h"
#include "movement.h"
#include "nutils.h"
#include "sdl_boilerplate.h"
#include "blockproperties.h"

// internal C libraries
#include <stdbool.h>

// internal C++ libraries
#include <iostream>

// Texture initialization
SDL_Texture *textures[4];
SDL_Texture *entityTextures[1];

std::string GamePath;
bool running;

SDL_GameController* gameController = nullptr;
SDL_Event event;
SDL_Renderer *renderer;
SDL_Rect dest_rect;
SDL_Window *window;
int windowWidth, windowHeight;
SDL_Surface *image;

int joyxa;
int joyya;
int joyxb;
int joyyb;

const Uint8 *keyboard_state;
Uint32 delta;

float camx = 0;
float camy = 4096;
int seed = 0;

int8_t selectorMode = 0;
int8_t oldSelectorMode = 0;

int xMouse;
int yMouse;
bool leftMouse;
bool rightMouse;

// arrays
int16_t world[128][4096];

// functions
SDL_Texture* nLoadTexture(std::string Path);
void generateWorld(FastNoiseLite tempnoise);
void renderBlocks();
void tickEntities();
int16_t getTile(float getTileX, float getTileY);

Entity* entities[1024];

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    const float xspeed = 0.02;
    //std::cout << "Enter world seed:";
    //std::cin >> seed;
    seed = 1;

    FastNoiseLite noise(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    nStartUp();
    initializeBlockProperties();

    entityTextures[0] = nLoadTexture("player.png");

    // read block properties and load their textures
    for (std::map<int, BlockProperties>::iterator it = blockProperties.begin(); it != blockProperties.end(); ++it) {
        int blockID = it->first;
        const auto& mapTexture = it->second.texture;
        textures[blockID] = nLoadTexture(mapTexture);
        // Use the blockID and texture as needed
        // ...
    }

    /*textures[0] = nLoadTexture("selector.png");
    textures[1] = nLoadTexture("grass.png");
    textures[2] = nLoadTexture("dirt.png");
    textures[3] = nLoadTexture("stone.png");*/

    generateWorld(noise);
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time;

    // spawn player
    entities[0] = new EntityPlayer();
    entities[0]->id = 1;
    entities[0]->x = 8 * 64;
    entities[0]->y = 66 * 64;

    int frameCount = 0;
    Uint32 lastFrameTime = SDL_GetTicks();

    while(running) {

    // Calculate FPS
        Uint32 currentFrameTime = SDL_GetTicks();
        Uint32 elapsedFrameTime = currentFrameTime - lastFrameTime;

        if (elapsedFrameTime >= 1000) {
            // One second has elapsed, so calculate FPS
            float fps = frameCount / (elapsedFrameTime / 1000.0f);

            // Print the FPS value to the console
            //if(vsync == 0) { std::cout << "FPS: " << fps << "\n"; }

            // Reset the frame count and last frame time
            frameCount = 0;
            lastFrameTime = currentFrameTime;
        }
        frameCount++;

        current_time = SDL_GetTicks();
        delta = current_time - last_time;
        last_time = current_time;
        //std::cout << (bool) SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A) << "\n";
        /*
        bool isAPressed = SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A);
if (isAPressed) {
    // Handle 'A' button press
}
        You can replace SDL_CONTROLLER_BUTTON_A with any of the following constants to check for other buttons on the game controller:

    SDL_CONTROLLER_BUTTON_B
    SDL_CONTROLLER_BUTTON_X
    SDL_CONTROLLER_BUTTON_Y
    SDL_CONTROLLER_BUTTON_BACK
    SDL_CONTROLLER_BUTTON_GUIDE
    SDL_CONTROLLER_BUTTON_START
    SDL_CONTROLLER_BUTTON_LEFTSTICK
    SDL_CONTROLLER_BUTTON_RIGHTSTICK
    SDL_CONTROLLER_BUTTON_LEFTSHOULDER
    SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
    SDL_CONTROLLER_BUTTON_DPAD_UP
    SDL_CONTROLLER_BUTTON_DPAD_DOWN
    SDL_CONTROLLER_BUTTON_DPAD_LEFT
    SDL_CONTROLLER_BUTTON_DPAD_RIGHT

        SDL_CONTROLLER_BUTTON_LEFTSHOULDER: for the left shoulder button
    SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: for the right shoulder button
    SDL_CONTROLLER_BUTTON_LEFT_TRIGGER: for the left trigger
    SDL_CONTROLLER_BUTTON_RIGHT_TRIGGER: for the right trigger

    shoulder is the hotbar, trigger is the action

        Triangle button -> SDL_CONTROLLER_BUTTON_Y
    Circle button -> SDL_CONTROLLER_BUTTON_B
    Cross button -> SDL_CONTROLLER_BUTTON_A
    Square button -> SDL_CONTROLLER_BUTTON_X

        SELECT button -> SDL_CONTROLLER_BUTTON_BACK
    START button -> SDL_CONTROLLER_BUTTON_START
    PS button -> SDL_CONTROLLER_BUTTON_GUIDE
        */

        if (keyboard_state[SDL_SCANCODE_D]) { entities[0]->speedX += xspeed * delta; }
        if (keyboard_state[SDL_SCANCODE_A]) { entities[0]->speedX -= xspeed * delta; }
        joyxa = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX);
        joyya = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY);

        camx += (entities[0]->x - camx) * delta/128;
        camy += (entities[0]->y - camy) * delta/128;

        SDL_SetRenderDrawColor(renderer, 180, 200, 255, 255);
        SDL_RenderClear(renderer);
        renderBlocks();
        tickEntities();

        nMouseTick();

        int selectedX = (64 * floor( (xMouse + camx) / 64 )) - camx + nmod(windowWidth/2,64);
        int selectedY = windowHeight -
                        ((64 * floor( (windowHeight - yMouse + 16 + camy) / 64) ) -
                        camy + nmod(windowHeight/2,64));

        if(leftMouse or ((bool) SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A))) {
            int selectedBlockY = std::max((int) floor((camy - selectedY) / 64) + (windowHeight / 128) + 1, 0);
            int selectedBlockX = std::max((int) floor((selectedX + camx) / 64) - (windowWidth  / 128) + 1, 0);
            int16_t currentBlock = world[selectedBlockY][selectedBlockX];
            if(selectorMode == 0) {
                if(currentBlock == 0) {
                    selectorMode = -1;
                } else {
                    selectorMode = 1;
                }
            }
            if(selectorMode == 1) {
                world[selectedBlockY][selectedBlockX] = 0;
            } else {
                world[selectedBlockY][selectedBlockX] = 1;
            }
        } else {
            selectorMode = 0;
        }

        SDL_Rect dest_rect = {
            (int) (selectedX),
            (int) (selectedY),
            65, 64 };
        SDL_RenderCopyEx(renderer, textures[0], NULL, &dest_rect, 0, NULL, SDL_FLIP_NONE);

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
        }
        entities[0]->speedX += xspeed * (joyxa/32767) * delta;
        SDL_RenderPresent(renderer);
    }

    // Destroy the texture, renderer, and window
    for(int i = 0; i < (sizeof(entities) / sizeof(Entity)); i++) {
        delete entities[i];
    }
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
    SDL_Rect dest_rect;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    for(int i = (windowWidth / -128); i < ((windowWidth / 64) + 3); i++) {
        for(int j = (windowHeight / -128); j < (windowHeight / 64) + 3; j++) {
            dest_rect = { (windowWidth / 2) + (64 * i) - nmod(camx, 64) - 64,
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
    if(getTileY > (127 * 64)) { return 0; }
    return world[(int) floor(getTileY / 64)][(int) floor(getTileX / 64)];
}

SDL_Texture* nLoadTexture(std::string Path)
{
    std::string TempPath = GamePath;
    image = IMG_Load(TempPath.append(Path).c_str());
    //std::cout << "Loaded texture:" << TempPath << "\n";
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return texture;
}

void tickEntities()
{
    for(int e = 0; e < 1024; e++) {
        if(entities[e] == nullptr) {
            break;
        }
        entities[e]->move();
        entities[e]->speedX *= 1 - (delta * (1 - 0.98));
        entities[e]->speedY -= 0.01 * delta;
        if(entities[e]->speedX < 0) {
            entities[e]->direction = -1;
        } else if(entities[e]->speedX > 0) {
            entities[e]->direction = 1;
        }
        SDL_RendererFlip flip;
        if (entities[e]->direction < 0) {
            flip = SDL_FLIP_HORIZONTAL;
        } else {
            flip = SDL_FLIP_NONE;
        }
        SDL_Rect dest_rect = { (int) ((windowWidth / 2) + entities[e]->x - camx - 16), (int) ((windowHeight / 2) + camy - entities[e]->y - 64), 8*4, 32*4 };
        SDL_RenderCopyEx(renderer, entityTextures[entities[e]->id - 1], NULL, &dest_rect, 0, NULL, flip);
    }
}
