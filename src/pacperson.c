#include "pacperson.h"
#include <ncurses.h>


void pacman(unsigned int width, unsigned int height) {
    object_t map[height][width];

    memset(map, WALL, height*width*sizeof(object_t));

    printw("pacman! %u %u\n", height, width);
    getch();
    




}