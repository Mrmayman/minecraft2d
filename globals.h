#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "FastNoiseLite.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern bool running;
extern const Uint8 *keyboard_state;
extern SDL_GameController* gameController = nullptr;
extern std::string GamePath;
extern SDL_Surface *image;

#endif // GLOBALS_H

