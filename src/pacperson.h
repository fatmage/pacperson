#ifndef PACPERSON_H
#define PACPERSON_H

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define DEFAULT_HEIGHT 21
#define DEFAULT_WIDTH  31


#define _2D_TO_1D(y, x, width) (((y) * (width)) + ((x) % (width)))
#define _1D_TO_X(pos, width) ((pos) % (width))
#define _1D_TO_Y(pos, width) ((pos) / (width))

#define PROGRAM_VERSION "0.1.0"

typedef enum object_t {
    EMPTY_FIELD = 0,
    WALL = 1,
    FOOD = 2,
    GHOST = 3,
    PLAYER = 4
} object_t;

#define LEFT_DIR  0
#define RIGHT_DIR 1
#define UP_DIR    2
#define DOWN_DIR  3



void pacman(uint8_t height, uint8_t width, int random_flag);
void move_ghosts();
int  move_player();

void pause_game();
void end_game();

void create_default_map();
void create_random_map();

#endif
