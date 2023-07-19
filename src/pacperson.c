#include "pacperson.h"
#include "drawing.h"


static object_t *map;
static uint8_t map_width, map_height;

static uint8_t playery = 1, playerx = 1; 
static uint16_t points = 0;
static uint8_t current_direction;
static uint8_t desired_direction;

static int directions[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

static WINDOW* map_window;

#define GAME_FRAME_TIME (1000.0f/7)
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
        WALL, PLAYER, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, 
        WALL, FOOD, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, 
        WALL, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, WALL, 
        WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, 
        WALL, FOOD, WALL, FOOD, WALL, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, WALL, FOOD, WALL, FOOD, WALL, 
        WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, 
        WALL, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, GHOST, WALL, 
        WALL, FOOD, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, 
        WALL, FOOD, WALL, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, WALL, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, WALL, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, WALL, FOOD, WALL, 
        WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, FOOD, WALL, 
        WALL, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, 
        WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, 
        WALL, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, 
        WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, 
        WALL, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, 
        WALL, FOOD, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, FOOD, WALL, 
        WALL, FOOD, WALL, FOOD, FOOD, FOOD, GHOST, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, FOOD, WALL, 
        WALL, FOOD, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, FOOD, WALL, WALL, WALL, WALL, WALL, FOOD, WALL, 
        WALL, GHOST, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, GHOST, WALL, 
        WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, 
    };

    for (int i = 0; i < map_height * map_width; i++) {
        map[i] = tmp_map[i];
    }

}

void move_ghosts() {

}


int move_player() {

    if (map[_2D_TO_1D(playery + directions[desired_direction][0], 
                        playerx + directions[desired_direction][1], map_width)] != WALL) {

        current_direction = desired_direction;
    }


    int new_playery = playery + directions[current_direction][0];
    int new_playerx = playerx + directions[current_direction][1];  
    int new_pos = _2D_TO_1D(new_playery, new_playerx, map_width);

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
            return 1;
        default:
            break;
    }

    return 0;
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

    map_window = newwin(map_height, map_width, map_win_start_y, map_win_start_x);
    nodelay(stdscr, TRUE);

    
    float prev_time = clock() * 1000.0f;
    prev_time /= (float)CLOCKS_PER_SEC;
    float time_elapsed = 0.0;
    int ghost_frames = 0;

    float milliseconds_elapsed = 0.0;

    print_header(points, floor(milliseconds_elapsed / 1000));
    print_map(map_window, map, map_height, map_width);
    print_footer(LINES - 1);
 
    // game loop
    while (true) {

        int pressed;

        switch (pressed = getch()) {
            case KEY_LEFT:
                desired_direction = LEFT_DIR;
                break;
            case KEY_RIGHT:
                desired_direction = RIGHT_DIR;
                break;
            case KEY_UP:
                desired_direction = UP_DIR;
                break;
            case KEY_DOWN:
                desired_direction = DOWN_DIR;
                break;
            case 'p':
            // Fall-through
            case 'P':
                pause_game();
                break;
            case 'q':
            // Fall-through
            case 'Q':
                goto GAME_OVER;
            default:
                break;
        }  

        float current_time = clock() * 1000;
        current_time /= (float)CLOCKS_PER_SEC;


        time_elapsed += current_time - prev_time;
        milliseconds_elapsed += current_time - prev_time;
        prev_time = current_time;

        if (time_elapsed > GAME_FRAME_TIME) {
            time_elapsed -= GAME_FRAME_TIME;
            ghost_frames++;

            if (ghost_frames > 4) {

                ghost_frames -= 4;
                move_ghosts();
            }

            if (move_player()) {
                end_game();
                return;
            }

            print_map(map_window, map, map_height, map_width);
            print_header(points, floor(milliseconds_elapsed / 1000));

            wrefresh(stdscr);
            wrefresh(map_window);
        }
    }


GAME_OVER:

    end_game();

}


void pause_game() {

}

void end_game() {
    free(map);
    delwin(map_window);
    endwin();

    printf("Wynik końcowy: %d\n", points);
}