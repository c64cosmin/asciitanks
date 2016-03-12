#ifndef _MAP_H_
#define _MAP_H_

#define MAP_EMPTY 0
#define MAP_DIRT 1
#define MAP_STONE 2
#define MAP_GRASS 3

typedef struct _map{
    char* map_data;
    int map_x;
    int map_y;
} map;

map new_map(int x, int y);
char get_map(map m, int x, int y);
void set_map(map m, int x, int y, char c);
#endif //_MAP_H_
