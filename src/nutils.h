#ifndef NUTILS_H
#define NUTILS_H

int nmod(int n, int m);
void nStartUp();
void nMouseTick();

extern int xMouse;
extern int yMouse;
extern bool leftMouse;
extern bool rightMouse;

const bool vsync = 1;

#endif  // NUTILS_H
