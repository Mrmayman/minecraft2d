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
// #include "sdl_boilerplate.h"
#include "blockproperties.h"
#include "tick.h"
#include "world.h"

// internal C libraries
#include <stdbool.h>

// internal C++ libraries
#include <iostream>

// Texture initialization

std::string GamePath;
bool running;

SDL_Event event;
SDL_Rect dest_rect;
SDL_Surface *image;
TTF_Font* font;

int seed = 0;

float delta;
int fps;
int xMouse;
int yMouse;
bool leftMouse;
bool rightMouse;

// functions
void generateWorld(FastNoiseLite tempnoise);
void renderBlocks();
void tickEntities();
void tickSelector();
void tickMovementAndFPS();

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    // std::cout << "Enter world seed:";
    // std::cin >> seed;
    seed = 1;

    FastNoiseLite noise(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    nStartUp();
    initializeBlockProperties();

    entityTextures[0] = nLoadTexture("player.png");

    // read block properties and load their textures
    for (std::map<int, BlockProperties>::iterator it = blockProperties.begin(); it != blockProperties.end(); ++it)
    {
        int blockID = it->first;
        const auto &mapTexture = it->second.texture;
        textures[blockID] = nLoadTexture(mapTexture);
    }

    /*textures[0] = nLoadTexture("selector.png");
    textures[1] = nLoadTexture("grass.png");
    textures[2] = nLoadTexture("dirt.png");
    textures[3] = nLoadTexture("stone.png");*/

    generateWorld(noise);
    last_time = SDL_GetTicks();

    // spawn player
    entities[0] = new EntityPlayer();
    entities[0]->id = 1;
    entities[0]->x = 8 * 64;
    entities[0]->y = 66 * 64;

    lastFrameTime = SDL_GetTicks();

    std::cout << "Started Game\n";
    while (running)
    {
        tickMovementAndFPS();
        SDL_SetRenderDrawColor(renderer, 180, 200, 255, 255);
        SDL_RenderClear(renderer);
        renderBlocks();
        tickEntities();
        nMouseTick();
        tickSelector();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        entities[0]->speedX += xspeed * (joyxa / 32767) * delta;
        nDrawText(13, 13, std::to_string(fps) + std::string(" fps"), {64,64,64});
        nDrawText(10, 10, std::to_string(fps) + std::string(" fps"), {255,255,255});
        SDL_RenderPresent(renderer);
    }

    // Destroy the texture, renderer, and window
    for (int i = 0; i < (sizeof(entities) / sizeof(Entity)); i++)
    {
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
    for (int j = 0; j < 4096; j++)
    {
        block = (int16_t)(64 + (tempnoise.GetNoise((float)j * 2, 1.0f) * 10.0));
        block += (int16_t)(tempnoise.GetNoise((float)j * 5, 1.0f) * 3.0);
        for (int i = block; i > -1; i--)
        {
            world[i][j] = 3;
        }
        world[block + 1][j] = 2;
        world[block + 2][j] = 2;
        world[block + 3][j] = 2;
        world[block + 4][j] = 1;
        for (int i = block + 5; i < 128; i++)
        {
            world[i][j] = 0;
        }
    }
}
