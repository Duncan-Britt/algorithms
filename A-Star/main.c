#include <stdio.h>
#include "grid.h"

#define MAX_PATH_SIZE 6000

#define GRID_STRING "\
................\n\
................\n\
...##......##...\n\
....#.......#...\n\
....#.......#...\n\
....###.....###.\n\
................\n\
........##......\n\
.........#......\n\
...##....####...\n\
....#.......#...\n\
....#.......#...\n\
....####....###.\n\
................\n\
................"
#define GRID_STRING_SIZE 255

int main() 
{
    printf("beginning A* test\n");

    Grid grid;

    char grid_string[GRID_STRING_SIZE] = GRID_STRING;
    int8_t err = grid_init_str(grid_string, &grid);
    if (err) {
        printf("err\n");
        free(grid.obstacles);
        return -1;
    }
    grid_print(&grid);
    printf("\n");

    uint16_t path[MAX_PATH_SIZE]; 
    uint16_t path_size = 0; 

    err = grid_find_path_a_star(&grid, 15, 224, path, &path_size, MAX_PATH_SIZE);
    switch (err) {
    case 0:
        grid_print_path(&grid, path, path_size);
        printf("\n");
        break;
    case 1:        
        printf("Path not found\n");
        break;
    case -1:        
        printf("Path exceeded max path size\n");
        grid_print_path(&grid, path, path_size);
        printf("\n");
        break;
    case -2:
        printf("Invalid Destination\n");
        break;
    case -3:
        printf("Invalid start\n");
        break;
    case -4:
        printf("Exceeded max priority queue size\n");
        break;
    case -5:
        printf("Exceeded max size for explored nodes array\n");
    }

    free(grid.obstacles);
    return 0;
}

