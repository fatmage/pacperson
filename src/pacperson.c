#include "pacperson.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>


static object_t *map;

void create_map(unsigned int height, unsigned int width) {
    map = calloc(height * width, sizeof(object_t));
    memset(map, WALL, height * width * sizeof(object_t));
}


void pacman(unsigned int height, unsigned int width) {

    create_map( height, width);

    printw("pacman! %u %u\n", height, width);
    getch();


}