#ifndef TICK_H
#define TICK_H

#include <SDL2/SDL.h>
#include "entity.h"

extern SDL_Window *window;
extern int windowWidth, windowHeight;
extern SDL_Renderer *renderer;

extern float camx;
extern float camy;

extern SDL_Texture *textures[4];
extern SDL_Texture *entityTextures[1];

extern int8_t selectorMode;
extern int8_t oldSelectorMode;

extern Uint32 lastFrameTime;
extern int frameCount;
extern Uint32 delta;
extern Uint32 last_time;
extern Uint32 current_time;

extern const Uint8 *keyboard_state;
const float xspeed = 0.02;
extern SDL_GameController *gameController;

extern int joyxa;
extern int joyya;
extern int joyxb;
extern int joyyb;

#endif