#include <stdlib.h>
#include "map.h"

map new_map(int x, int y){
    map m;
    m.map_data = (char*)malloc(sizeof(char)*x*y);
    m.map_x = x;
    m.map_y = y;
    if(m.map_data == 0){
        m.map_x = 0;
        m.map_y = 0;
        return m;
    }
    int xi,yi;
    for(xi=0;xi<m.map_x;xi++)
    for(yi=0;yi<m.map_y;yi++)
        m.map_data[xi+yi*m.map_x] = 0;
    return m;
}

char get_map(map m, int x, int y){
    if(m.map_data==0)return 0;
    if(x<0||x>=m.map_x||y<0||y>=m.map_y)return 0;
    return m.map_data[x+y*m.map_x];
}

void set_map(map m, int x, int y, char c){
    if(m.map_data==0)return;
    if(x<0||x>=m.map_x||y<0||y>=m.map_y)return;
    m.map_data[x+y*m.map_x] = c;
}
