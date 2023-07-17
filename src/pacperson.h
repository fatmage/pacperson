#ifndef PACPERSON_H
#define PACPERSON_H

#include <string.h>

#define DEFAULT_HEIGHT 20
#define DEFAULT_WIDTH  30

#define PROGRAM_VERSION "0.0.1"

typedef enum object_t {
    EMPTY_FIELD = 0,
    WALL = 1,
    FOOD = 2,
    GHOST = 3,
    PLAYER = 4
} object_t;





void pacman(unsigned int height, unsigned int width);

#endif
