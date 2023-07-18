#include "pacperson.h"
#include "drawing.h"


static object_t *map;
static uint8_t map_width, map_height;

static uint8_t playery = 1, playerx = 1; 
static uint16_t points = 0;

#define GAME_FRAME_TIME (1.0f/30)
#define GHOST_SPEED 10

void create_random_map() {

    map = calloc(map_height * map_width, sizeof(object_t));
    memset(map, EMPTY_FIELD, map_height * map_width * sizeof(object_t));

    for (int i = 0; i < map_width; i++) {
        map[_2D_TO_1D(0, i, map_width)] = WALL;
        map[_2D_TO_1D(map_height - 1, i, map_width)] = WALL;
    }

    for (int i = 0; i < map_height; i++) {
        map[_2D_TO_1D(i, 0, map_width)] = WALL;
        map[_2D_TO_1D(i, map_width - 1, map_width)] = WALL;
    }

    int wall_count[map_height][map_width];

    for (int y = 0; y < map_height; y++) {
        wall_count[y][0] = 2;
        wall_count[y][map_width - 1] = 2;
    }

    for (int x = 0; x < map_width; x++) {
        wall_count[0][x] = 2;
        wall_count[map_height - 1][x] = 2;
    }

    for (int y = 1; y < map_height - 1; y++) {
        for (int x = 1; x < map_width - 1; x++) {
            int sum = (int)map[_2D_TO_1D(y - 1, x, map_width)] + (int)map[_2D_TO_1D(y, x - 1, map_width)] +
                      (int)map[_2D_TO_1D(y + 1, x, map_width)] + (int)map[_2D_TO_1D(y, x + 1, map_width)];
            wall_count[y][x] = sum;
        }
    }

    for (int y = 1; y < map_height - 1; y++) {
        for (int x = 1; x < map_width - 1; x++) {
            if (wall_count[y - 1][x] < 3 && wall_count[y][x - 1] < 3 && 
                wall_count[y + 1][x] < 3 && wall_count[y][x + 1] < 3) {

                if (wall_count[y - 1][x] == 0 || wall_count[y][x - 1] == 0) {

                    map[_2D_TO_1D(y, x, map_width)] = WALL;
                    wall_count[y - 1][x]++;
                    wall_count[y + 1][x]++;
                    wall_count[y][x - 1]++;
                    wall_count[y][x + 1]++;                                         
                } else {
                    int rand_res;
                    if ((rand_res = rand() % 100) < 75)  {
                        map[_2D_TO_1D(y, x, map_width)] = WALL;
                        wall_count[y - 1][x]++;
                        wall_count[y + 1][x]++;
                        wall_count[y][x - 1]++;
                        wall_count[y][x + 1]++;
                    }
                }
            }
        }
    }

}

void create_default_map() {
    map = calloc(map_height * map_width, sizeof(object_t));
    memset(map, WALL, map_height * map_width * sizeof(object_t));

    object_t tmp_map[] = {
        WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, 
        WALL, PLAYER, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, GHOST, WALL, 
        WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, GHOST, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, WALL, EMPTY_FIELD, WALL, 
        WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, EMPTY_FIELD, WALL, WALL, WALL, WALL, WALL, EMPTY_FIELD, WALL, 
        WALL, GHOST, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, EMPTY_FIELD, GHOST, WALL, 
        WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, 
    };

    for (int i = 0; i < map_height * map_width; i++) {
        map[i] = tmp_map[i];
    }


}



void pacman(uint8_t height, uint8_t width, int random_flag) {

    // initialisation
    srand(time(NULL));
    curses_init();

    if (random_flag) { 
        map_height = height;
        map_width = width;
        create_random_map();
    } else {
        map_height = DEFAULT_HEIGHT;
        map_width  = DEFAULT_WIDTH;
        create_default_map();
    }


    int map_win_start_y = (LINES - map_height)/2;
    int map_win_start_x = (COLS - map_width)/2;

    WINDOW *map_window = newwin(map_height, map_width, map_win_start_y, map_win_start_x);
    nodelay(stdscr, TRUE);

    print_header();
    print_map(map_window, map, map_height, map_width);
    print_footer(LINES - 1);

    
    int prev_time = clock() / (CLOCKS_PER_SEC / 1000);
    float time_elapsed = 0.0;
    int ghost_frames = 0;
 
    // game loop
    while (true) {

        int pressed;

        time_elapsed += (clock() / CLOCKS_PER_SEC) - prev_time;

        if (2 > GAME_FRAME_TIME) {
            time_elapsed -= GAME_FRAME_TIME;
            ghost_frames++;

            if (ghost_frames > 10) {
                // move ghosts
                ghost_frames -= 10;

            }

            int new_pos;
            int new_playery = playery;
            int new_playerx = playerx;

            switch (pressed = getch()) {
                case KEY_LEFT:
                    new_playerx -= 1;
                    goto MOVE_PLAYER;
                case KEY_RIGHT:
                    new_playerx += 1;
                    goto MOVE_PLAYER;
                case KEY_UP:
                    new_playery -= 1;
                    goto MOVE_PLAYER;
                case KEY_DOWN:
                    new_playery += 1;
             
MOVE_PLAYER:
                    new_pos = _2D_TO_1D(new_playery, new_playerx, map_width);
                    switch (map[new_pos]) {
                        case FOOD:
                            points++;
                        case EMPTY_FIELD:
                            map[new_pos] = PLAYER;
                            map[_2D_TO_1D(playery, playerx, map_width)] = EMPTY_FIELD;
                            playery = new_playery;
                            playerx = new_playerx;
                            break;
                        case GHOST:
                            goto GAME_OVER;
                        default:
                            break;
                    }
                    break;
                case 'q':
                // Fall-through
                case 'Q':
                    // PAUZA
                    break;
                default:
                    break;
            }      

        }


        print_map(map_window, map, map_height, map_width);

        wrefresh(stdscr);
        wrefresh(map_window);


    }


GAME_OVER:



    free(map);
    delwin(map_window);
    endwin();

}