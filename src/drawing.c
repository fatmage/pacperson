#include "pacperson.h"
#include "drawing.h"

// ncurses initialization
void curses_init() {

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    start_color();
    init_pair(WALL,   COLOR_BLACK,  COLOR_WHITE);
    init_pair(GHOST,  COLOR_RED,    COLOR_BLACK);
    init_pair(PLAYER, COLOR_YELLOW, COLOR_BLACK);
    init_pair(FOOD,   COLOR_GREEN,  COLOR_BLACK);
    curs_set(0);

}

void print_header(uint16_t points, uint64_t time, uint16_t window_width) {
    char points_str[20] = {};
    char time_str[25] = {};

    sprintf(points_str, "Points: %d", points);
    sprintf(time_str, "Time: %ld:%ld", time / 60, time % 60);


    unsigned int points_pos = (COLS - window_width) / 2;
    unsigned int time_pos = points_pos + window_width - 11;

    mvprintw(1, points_pos,   "%s", points_str);
    mvprintw(1, time_pos, "%s", time_str);
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
                    waddstr(win, "  ");
                    break;
                case WALL:
                    waddch(win, ' ' | COLOR_PAIR(WALL));
                    waddch(win, ' ' | COLOR_PAIR(WALL));
                    break;
                case FOOD:
                    waddch(win, '<' | COLOR_PAIR(FOOD));
                    waddch(win, '>' | COLOR_PAIR(FOOD));
                    break;
                case GHOST:
                    waddch(win, '/' | COLOR_PAIR(GHOST));
                    waddch(win, '\\' | COLOR_PAIR(GHOST));
                    break;
                case PLAYER:
                    waddch(win, '(' | COLOR_PAIR(PLAYER));
                    waddch(win, 'K' | COLOR_PAIR(PLAYER)); 
                    break;
                default:
                    waddstr(win, "??");
                    break;
            }
        }

        wmove(win, y+1, 0);
    }

}