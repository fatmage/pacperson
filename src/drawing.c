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

void print_header() {
    const char *title = "PacPerson";
    int col = getmaxx(stdscr);

    mvprintw(0, (col-strlen(title))/2, "%s", title);
}

void print_footer(uint8_t offset) {
    const char *footer = "v. "PROGRAM_VERSION" fatm4ge";
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
                    waddch(win, 'o');
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