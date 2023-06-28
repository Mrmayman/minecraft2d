#include <string>
#include <SDL2/SDL_ttf.h>

int nmod(int n, int m);
void nStartUp();
void nMouseTick();
SDL_Texture *nLoadTexture(std::string Path);
void nDrawText(int textX, int textY, std::string textString, SDL_Color color);

extern int xMouse;
extern int yMouse;
extern bool leftMouse;
extern bool rightMouse;
extern TTF_Font* font;

const bool vsync = 1;
