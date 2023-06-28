#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>

#include "entity.h"
#include "tile.h"
#include "movement.h"
#include "nutils.h"
#include "sdl_boilerplate.h"
#include "tick.h"

void Entity::tick()
{
    move();
    speedX *= 1 - (delta * (1 - 0.98));
    speedY -= 0.01 * delta;
    if (speedX < 0) {
        direction = -1;
    } else if (speedX > 0) {
        direction = 1;
    }
    SDL_RendererFlip flip;
    if (direction < 0) {
        flip = SDL_FLIP_HORIZONTAL;
    } else {
        flip = SDL_FLIP_NONE;
    }
    SDL_Rect dest_rect = {
        (int)((windowWidth / 2) + x - camx - 16),
        (int)((windowHeight / 2) + camy - y - 64),
        width, height
    };
    SDL_RenderCopyEx(renderer, entityTextures[id - 1], NULL, &dest_rect, 0, NULL, flip);
}

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

    std::cout << y << "\n";
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
    std::cout << speedX << "\n";
    // x collision
    if(getTile(x, y) > 0) {
        while(getTile(x, y) > 0) {
            if(speedX < 0) {
                x++;
            } else {
                x--;
            }
            std::cout << x << "\n";
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
