#include "pacperson.h"
#include "drawing.h"


static object_t *map;
static uint8_t map_width, map_height;

static uint8_t playery = 1, playerx = 1; 
static uint16_t points = 0;
static uint8_t current_direction;
static uint8_t desired_direction;
static uint8_t ghosts[4][2];
static uint8_t ghost_directions[4];
static object_t ghosts_floor[4] = {FOOD, FOOD, FOOD, FOOD};

static int directions[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

static WINDOW* map_window;


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
        WALL, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, FOOD, WALL, 
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

    ghosts[0][0] = map_height - 2;
    ghosts[0][1] = 1;
    ghost_directions[0] = UP_DIR;

    ghosts[1][0] = 1;
    ghosts[1][1] = map_width - 2;
    ghost_directions[1] = LEFT_DIR;

    ghosts[2][0] = map_height - 2;
    ghosts[2][1] = map_width - 2;
    ghost_directions[2] = UP_DIR;

    ghosts[3][0] = map_height - 4;
    ghosts[3][1] = 6;
    ghost_directions[3] = LEFT_DIR;

}

void move_ghosts() {

    uint8_t turn_directions[4] = {LEFT_DIR, RIGHT_DIR, UP_DIR, DOWN_DIR};

    uint8_t possible_directions[4];

    for (int curr_ghost = 0; curr_ghost < 4; curr_ghost++) {

        // fill possible_directions array
        int j = 0;
        for (int i = 0; i < 4; i++) {
            switch(ghost_directions[curr_ghost]) {
                case LEFT_DIR:
                    possible_directions[3] = RIGHT_DIR;
                    if (turn_directions[i] != RIGHT_DIR) {
                        possible_directions[j] = turn_directions[i];
                        j++;
                    }
                    break;
                case RIGHT_DIR:
                    possible_directions[3] = LEFT_DIR;
                    if (turn_directions[i] != LEFT_DIR) {
                        possible_directions[j] = turn_directions[i];
                        j++;
                    }
                    break;
                case UP_DIR:
                    possible_directions[3] = DOWN_DIR;
                    if (turn_directions[i] != DOWN_DIR) {
                        possible_directions[j] = turn_directions[i];
                        j++;
                    }
                    break;
                case DOWN_DIR:
                    possible_directions[3] = UP_DIR;
                    if (turn_directions[i] != UP_DIR) {
                        possible_directions[j] = turn_directions[i];
                        j++;
                    }
                    break;
            }
        }

        for (int i = 0; i < 2; i++) {
            int j = i + rand() / (RAND_MAX / (3 - i) + 1);
            uint8_t tmp = possible_directions[j];
            possible_directions[j] = possible_directions[i];
            possible_directions[i] = tmp;
        }

        int new_pos = 0;
        for (int i = 0; i < 4; i++) {
            new_pos = _2D_TO_1D(ghosts[curr_ghost][0] + directions[possible_directions[i]][0],
                                ghosts[curr_ghost][1] + directions[possible_directions[i]][1], map_width);
            if (map[new_pos] == EMPTY_FIELD || map[new_pos] == FOOD || map[new_pos] == GHOST) {
                ghost_directions[curr_ghost] = possible_directions[i];
                break;
            }
        }



        map[_2D_TO_1D(ghosts[curr_ghost][0], ghosts[curr_ghost][1], map_width)] = ghosts_floor[curr_ghost];
        if (map[new_pos] == GHOST) {
            for (int i = 0; i < 4; i++) {
                if (_2D_TO_1D(ghosts[i][0], ghosts[i][1], map_width) == new_pos) {
                    ghosts_floor[curr_ghost] = ghosts_floor[i];
                }
            }
        } else {
            ghosts_floor[curr_ghost] = map[new_pos];
        }

        map[new_pos] = GHOST;
        ghosts[curr_ghost][0] += directions[ghost_directions[curr_ghost]][0];
        ghosts[curr_ghost][1] += directions[ghost_directions[curr_ghost]][1];

    }




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

            if (ghost_frames > GHOST_SPEED) {

                ghost_frames -= GHOST_SPEED;
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

    printf("Final result: %d\n", points);
}