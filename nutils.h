#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "FastNoiseLite.h"
#include "globals.h"

void nStartUp(FastNoiseLite tempnoise);
int nmod(int n, int m);
SDL_Texture* nLoadTexture(std::string Path);
