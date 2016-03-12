#include "map.h"

map new_map(int x, int y){
    map m;
    m.map_data = (char*)malloc(sizeof(char)*x*y);
    m.map_x = x;
    m.map_y = y;
}

char get_map(map m, int x, int y){
    if(m.map_data==0)return 0;
    return m.map_data[x+y*m.map_data];
}

void set_map(map m, int x, int y, char c){
    if(m.map_data==0)return;
    m.map_data[x+y*m.map_data] = c;
}
