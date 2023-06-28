#include <stdint.h>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "world.h"

Entity *entities[1024];
int16_t world[128][4096];

int16_t getTile(float getTileX, float getTileY)
{
    if (getTileX < 0) {
        return 0;
    }
    if(getTileX > (4095 * 64)) {
        return 0;
    }
    if (getTileY < 0) {
        return 0;
    }
    if (getTileY > (127 * 64)) {
        return 0;
    }
    return world[(int)floor(getTileY / 64)][(int)floor(getTileX / 64)];
}
