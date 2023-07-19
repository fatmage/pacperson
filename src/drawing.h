#ifndef DRAWING_H
#define DRAWING_H


void curses_init();

void print_map(WINDOW *win, object_t *map, uint8_t h, uint8_t w);
void print_header(uint16_t points, uint64_t time);
void print_footer(uint8_t offset); 


#endif