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

// internal C libraries
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

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
SDL_GameController* gameController = nullptr;

int joyxa;
int joyya;
int joyxb;
int joyyb;

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
const bool vsync = 1;

struct Entity {
    int id = 0;
    float x, y, speedX, speedY, newx, newy;
    int16_t direction = 1;
};
Entity entities[1024];

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
            printf("FPS: %f\n", fps);
    
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
        
        if (keyboard_state[SDL_SCANCODE_D]) { entities[0].speedX += xspeed * delta; }
        if (keyboard_state[SDL_SCANCODE_A]) { entities[0].speedX -= xspeed * delta; }
        joyxa = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX);
        joyya = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY);

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
            if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A)) {
                // Handle button A press
            }
        }
        entities[0].speedX += xspeed * (joyxa/32767) * delta;
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

int nmod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
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

void nStartUp(FastNoiseLite tempnoise)
{
    tempnoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    
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

void tickEntities(Uint32 tempdelta)
{
    for(int e = 0; e < 1024; e++) {
        if(entities[e].id == 0) {
            break;
        }
        moveEntity(e, tempdelta);
        entities[e].speedX *= 1 - (tempdelta * (1 - 0.98));
        entities[e].speedY -= 0.01 * tempdelta;
        if(entities[e].speedX < 0) {
            entities[e].direction = -1;
        } else if(entities[e].speedX > 0) {
            entities[e].direction = 1;
        }
        SDL_RendererFlip flip;
        if (entities[e].direction < 0) {
            flip = SDL_FLIP_HORIZONTAL;
        } else {
            flip = SDL_FLIP_NONE;
        }
        SDL_Rect dest_rect = { (int) ((windowWidth / 2) + entities[e].x - camx - 16), (int) ((windowHeight / 2) + camy - entities[e].y - 64), 8*4, 32*4 };
        SDL_RenderCopyEx(renderer, entityTextures[entities[e].id - 1], NULL, &dest_rect, 0, NULL, flip);
    }
}

void moveEntity(int tempentity, Uint32 tempdelta2)
{
    entities[tempentity].x += 0.4 * entities[tempentity].speedX * tempdelta2;
    
    // x collision
    if(getTile(entities[tempentity].x, entities[tempentity].y) > 0) {
        while(getTile(entities[tempentity].x, entities[tempentity].y) > 0) {
            if(entities[tempentity].speedX < 0) {
                entities[tempentity].x++;
            } else {
                entities[tempentity].x--;
            }
            entities[tempentity].x = floor(entities[tempentity].x);
        }
        entities[tempentity].speedX = 0;
    }
    
    entities[tempentity].y += 0.4 * entities[tempentity].speedY * tempdelta2;
    
    // y collision
    if(getTile(entities[tempentity].x, entities[tempentity].y) > 0) {
        while(getTile(entities[tempentity].x, entities[tempentity].y) > 0) {
            if(entities[tempentity].speedY < 0) {
                entities[tempentity].y++;
            } else {
                entities[tempentity].y--;
            }
            entities[tempentity].y = floor(entities[tempentity].y);
        }
        if(entities[tempentity].id == 1 && (keyboard_state[SDL_SCANCODE_W] or (joyya < -16000)) && entities[tempentity].speedY < 0) {
            entities[tempentity].speedY = 3 * nmod(tempdelta2,2);
        } else {
            entities[tempentity].speedY = 0;
        }
    }
}
