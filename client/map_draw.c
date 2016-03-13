#include <math.h>
#include "map_draw.h"
#include "gfx.h"

char* dirt_sprite[3];
char* stone_sprite[4];
char* grass_sprite[5];

int dirt_symbol[3];
int stone_symbol[4];
int grass_symbol[5];

int random(int x, int d){
    double r = cos((double)x*125.984d+616.414d)*568.486d + cos((double)x*483.165d+448.168d)*468.168d;
    r = r - floor(r);
    return (int)floor(r*d);
}

void map_draw_init(){
    dirt_sprite[0] = " ";
    dirt_sprite[1] = "\u2591";
    dirt_sprite[2] = "\u2592";

    stone_sprite[0] = " ";
    stone_sprite[1] = "\u00b0";
    stone_sprite[2] = "\u221e";
    stone_sprite[3] = "\u22c4";

    grass_sprite[0] = "<";
    grass_sprite[1] = ">";
    grass_sprite[2] = "x";
    grass_sprite[3] = "\u2264";
    grass_sprite[4] = "\u2265";

    int i;
    for(i=0;i<3;i++){
        dirt_symbol[i] = i;
        gfx_set_symbol(i, dirt_sprite[i]);
    }
    for(i=0;i<4;i++){
        stone_symbol[i] = i+3;
        gfx_set_symbol(i+3, stone_sprite[i]);
    }
    for(i=0;i<5;i++){
        grass_symbol[i] = i+7;
        gfx_set_symbol(i+7, grass_sprite[i]);
    }
}

void map_draw(map m, int camera_x, int camera_y){
    int x,y;
    for(x=0;x<MAP_SCREEN_X;x++)
    for(y=0;y<MAP_SCREEN_Y;y++){
        char cell = get_map(m, x + camera_x, y + camera_y);
        int put = ' ';
        int bg = BLACK;
        int fg = RED|GREEN|BLUE;
        int seed = (x + camera_x)+(y + camera_y)*m.map_x;
        if(cell == MAP_DIRT){
            put = dirt_symbol[random(seed, 3)];
            bg = RED;
            fg = BLACK;
        }
        if(cell == MAP_STONE){
            put = stone_symbol[random(seed, 4)];
            bg = RED|GREEN|BLUE;
            fg = BLACK|BRIGHT;
        }
        if(cell == MAP_GRASS){
            put = grass_symbol[random(seed, 5)];
            bg = GREEN;
            fg = GREEN|BRIGHT;
        }
        gfx_put(x, y, put, bg, fg);
    }
}
