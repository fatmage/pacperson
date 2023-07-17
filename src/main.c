#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>

#include "pacperson.h"

void print_version() {

    printf("pacperson version "PROGRAM_VERSION" author fatmage fatm4ge@gmail.com\n");
}

void print_help() {

    const char* helpstring = "\
Usage: pacperson [OPTIONS] ...\n\
\n\
Options:\n\
    -h, --help\n\
        print this help information.\n\
    -v, --version\n\
        print version information.\n\
    -S N, --size=N\n\
        set width and height of the map to N.\n\
    -w N, --width=N\n\
        set width of the map to N.\n\
    -h N, --height=N\n\
        set height of the map to N.\n";

    printf("%s\n", helpstring);
    print_version();
}






int main(int argc, char **argv) {

    struct option options[] = {
        {"version", no_argument, NULL, 'v'},
        {"help",    no_argument, NULL, 'h'},
        {"width",  no_argument, NULL, 'W'},
        {"height",     no_argument, NULL, 'H'},
        {"size", no_argument, NULL, 'S'},
        {0, 0, 0, 0}
    };

    int c = 0;


    unsigned int height = DEFAULT_HEIGHT, width = DEFAULT_WIDTH;
    


    while ((c = getopt_long(argc, argv, "hvH;S;W;", options, NULL)) != -1) {

        if (c == '?') {
            print_help();
            return EXIT_FAILURE;
        }

        unsigned int atoi_res;

        switch (c) {
            case 'S':
                atoi_res = atoi(argv[optind]);
                
                height = width = atoi_res < 15 ? atoi_res : 15;
                break;
            case 'W':
                atoi_res = atoi(argv[optind]);
                
                width = atoi_res < 15 ? atoi_res : 15;
                break;
            case 'H':
                atoi_res = atoi(argv[optind]);
                
                height = atoi_res < 15 ? atoi_res : 15;
                break;
            case 'v':
                print_version();
                return EXIT_SUCCESS;
            case 'h':
                print_help();
                return EXIT_SUCCESS;
            default:
                print_help  ();
                return EXIT_FAILURE;
        }
    }

    height |= 1;
    width  |= 1;

    initscr();
    pacman(height, width);
    endwin();
    


    return EXIT_SUCCESS;
}