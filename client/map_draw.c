#include "map_draw.h"
#include "gfx.h"

char* dirt_sprite[3];
char* stone_sprite[4];
char* grass_sprite[5];

int dirt_symbol[3];
int stone_symbol[4];
int grass_symbol[5];

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
    for(x=0;x<100;x++)
    for(y=0;y<70;y++)
        gfx_put(x,y,(x+y)%3+0,GREEN,GREEN|BRIGHT);
}
