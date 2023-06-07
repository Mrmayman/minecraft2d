#include <iostream>
#include <SDL2/SDL.h>

#include "entity.h"
#include "tile.h"
#include "movement.h"
#include "nutils.h"

void Entity::move()
{
    x += 0.4 * speedX * delta;

    // x collision
    if(getTile(x, y) > 0) {
        while(getTile(x, y) > 0) {
            if(speedX < 0) {
                x++;
            } else {
                x--;
            }
            x = floor(x);
        }
        speedX = 0;
    }

    y += 0.4 * speedY * delta;

    // y collision
    if(getTile(x, y) > 0) {
        while(getTile(x, y) > 0) {
            if(speedY < 0) {
                y++;
            } else {
                y--;
            }
            y = floor(y);
        }
        speedY = 0;
    }
}

void EntityPlayer::move() {
    x += 0.4 * speedX * delta;

    // x collision
    if(getTile(x, y) > 0) {
        while(getTile(x, y) > 0) {
            if(speedX < 0) {
                x++;
            } else {
                x--;
            }
            x = floor(x);
        }
        speedX = 0;
    }

    y += 0.4 * speedY * delta;

    // y collision
    if(getTile(x, y) > 0) {
        while(getTile(x, y) > 0) {
            if(speedY < 0) {
                y++;
            } else {
                y--;
            }
            y = floor(y);
        }
        if(id == 1 && (keyboard_state[SDL_SCANCODE_W] or (joyya < -16000)) && speedY < 0) {
            speedY = 3 * nmod(delta,2);
        } else {
            speedY = 0;
        }
    }
}
