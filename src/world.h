#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>
#include "entity.h"

extern Entity *entities[1024];
extern int16_t world[128][4096];
int16_t getTile(float getTileX, float getTileY);

#endif