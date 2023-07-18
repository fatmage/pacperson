#ifndef PACPERSON_H
#define PACPERSON_H

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define DEFAULT_HEIGHT 21
#define DEFAULT_WIDTH  31


#define _2D_TO_1D(y, x, width) (((y) * (width)) + ((x) % (width)))
#define _1D_TO_X(pos, width) ((pos) % (width))
#define _1D_TO_Y(pos, width) ((pos) / (width))

#define PROGRAM_VERSION "0.0.2"

typedef enum object_t {
    EMPTY_FIELD = 0,
    WALL = 1,
    FOOD = 2,
    GHOST = 3,
    PLAYER = 4
} object_t;



void pacman(uint8_t height, uint8_t width, int random_flag);
void create_default_map();
void create_random_map();

#endif
