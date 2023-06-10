#include <string>

int nmod(int n, int m);
void nStartUp();
void nMouseTick();
SDL_Texture *nLoadTexture(std::string Path);

extern int xMouse;
extern int yMouse;
extern bool leftMouse;
extern bool rightMouse;

const bool vsync = 1;
