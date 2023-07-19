#include "pacperson.h"
#include "drawing.h"

// ncurses initialization
void curses_init() {

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);

}

void print_header(uint16_t points, uint64_t time) {
    char points_str[20] = {};
    char time_str1[20] = {};
    char time_str2[5] = {};

    sprintf(points_str, "Points: %d", points);
    sprintf(time_str1, "Time: %ld", time / 60);
    sprintf(time_str2,": %ld", time % 60);


    int col = getmaxx(stdscr);

    mvprintw(1, col/7,   "%s", points_str);
    mvprintw(1, col*6/7, "%s", time_str1);
    mvprintw(1, col*6/7 + 8, "%s", time_str2);
}

void print_footer(uint8_t offset) {
    const char *footer = "PacPerson v. "PROGRAM_VERSION" fatm4ge";
    int col = getmaxx(stdscr);

    mvprintw(offset, (col-strlen(footer))/2, "%s", footer);
}

void print_map(WINDOW *win, object_t *map, uint8_t h, uint8_t w) {

    wmove(win, 0, 0);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            switch (map[_2D_TO_1D(y, x, w)]) {
                case EMPTY_FIELD:
                    waddch(win, ' ');
                    break;
                case WALL:
                    waddch(win, '#');
                    break;
                case FOOD:
                    waddch(win, '.');
                    break;
                case GHOST:
                    waddch(win, 'A');
                    break;
                case PLAYER:
                    waddch(win, 'G');
                    break;
                default:
                    waddch(win, '.');
                    break;
            }
        }

        wmove(win, y+1, 0);
    }

}