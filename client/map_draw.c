#include <math.h>
#include "map_draw.h"
#include "gfx.h"

char* dirt_sprite[3];
char* stone_sprite[4];
char* grass_sprite[5];
char* tank_sprite[17];

int dirt_symbol[3];
int stone_symbol[4];
int grass_symbol[5];
int tank_symbol[17];

char* tank_ch_u[7];
char* tank_fg_u[7];
char* tank_bg_u[7];

char* tank_ch_r[7];
char* tank_fg_r[7];
char* tank_bg_r[7];

char* tank_ch_d[7];
char* tank_fg_d[7];
char* tank_bg_d[7];

char* tank_ch_l[7];
char* tank_fg_l[7];
char* tank_bg_l[7];

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

    tank_sprite[0] = "\u25a0";//tank tracks//a

    tank_sprite[1] = "\u25b2";//triangles up//b
    tank_sprite[2] = "\u25b6";//triangles right//c
    tank_sprite[3] = "\u25bc";//triangles down//d
    tank_sprite[4] = "\u25c0";//triangles left//e

    tank_sprite[5] = "\u2565";//cannon tip up//f
    tank_sprite[6] = "\u2561";//cannon tip right//g
    tank_sprite[7] = "\u2568";//cannon tip down//h
    tank_sprite[8] = "\u255e";//cannon tip left//i

    tank_sprite[9] = "\u2550";//cannon horizontal//j
    tank_sprite[10]= "\u2551";//cannon vertical//k

    tank_sprite[11]= "\u250c";//frame top-left//l
    tank_sprite[12]= "\u2510";//frame top-right//m
    tank_sprite[13]= "\u2514";//frame bot-left//n
    tank_sprite[14]= "\u2518";//frame bot-right//o

    tank_sprite[15]= "\u2500";//frame horizontal//p
    tank_sprite[16]= "\u2502";//frame vertical//q

    tank_ch_u[0]="a  f  a";tank_fg_u[0]="i  p  i";tank_bg_u[0]="h  h  h";
    tank_ch_u[1]="aXXkXXa";tank_fg_u[1]="ihhphhi";tank_bg_u[1]="hiihiih";
    tank_ch_u[2]="aelhmca";tank_fg_u[2]="iXpppXi";tank_bg_u[2]="hihhhih";
    tank_ch_u[3]="aXqaqXa";tank_fg_u[3]="ihpXphi";tank_bg_u[3]="hihhhih";
    tank_ch_u[4]="aXnpoXa";tank_fg_u[4]="ihppphi";tank_bg_u[4]="hihhhih";
    tank_ch_u[5]="aXXXXXa";tank_fg_u[5]="ihhhhhi";tank_bg_u[5]="hiiiiih";
    tank_ch_u[6]="a     a";tank_fg_u[6]="i     i";tank_bg_u[6]="h     h";

    tank_ch_r[0]="aaaaaaa";tank_fg_r[0]="iiiiiii";tank_bg_r[0]="hhhhhhh";
    tank_ch_r[1]=" XXXbX ";tank_fg_r[1]=" hhhXh ";tank_bg_r[1]=" iiiii ";
    tank_ch_r[2]=" XlpmX ";tank_fg_r[2]=" hppph ";tank_bg_r[2]=" ihhhi ";
    tank_ch_r[3]=" Xqaijg";tank_fg_r[3]=" hpXppp";tank_bg_r[3]=" ihhhhh";
    tank_ch_r[4]=" XnpoX ";tank_fg_r[4]=" hppph ";tank_bg_r[4]=" ihhhi ";
    tank_ch_r[5]=" XXXdX ";tank_fg_r[5]=" hhhXh ";tank_bg_r[5]=" iiiii ";
    tank_ch_r[6]="aaaaaaa";tank_fg_r[6]="iiiiiii";tank_bg_r[6]="hhhhhhh";

    tank_ch_d[0]="a     a";tank_fg_d[0]="i     i";tank_bg_d[0]="h     h";
    tank_ch_d[1]="aXXXXXa";tank_fg_d[1]="ihhhhhi";tank_bg_d[1]="hiiiiih";
    tank_ch_d[2]="aXlpmXa";tank_fg_d[2]="ihppphi";tank_bg_d[2]="hihhhih";
    tank_ch_d[3]="aXqaqXa";tank_fg_d[3]="ihpXphi";tank_bg_d[3]="hihhhih";
    tank_ch_d[4]="aenfoca";tank_fg_d[4]="iXpppXi";tank_bg_d[4]="hihhhih";
    tank_ch_d[5]="aXXkXXa";tank_fg_d[5]="ihhphhi";tank_bg_d[5]="hiihiih";
    tank_ch_d[6]="a  h  a";tank_fg_d[6]="i  p  i";tank_bg_d[6]="h  h  h";

    tank_ch_l[0]="aaaaaaa";tank_fg_l[0]="iiiiiii";tank_bg_l[0]="hhhhhhh";
    tank_ch_l[1]=" XbXXX ";tank_fg_l[1]=" hXhhh ";tank_bg_l[1]=" iiiii ";
    tank_ch_l[2]=" XlpmX ";tank_fg_l[2]=" hppph ";tank_bg_l[2]=" ihhhi ";
    tank_ch_l[3]="ijgaqX ";tank_fg_l[3]="pppXph ";tank_bg_l[3]="hhhhhi ";
    tank_ch_l[4]=" XnpoX ";tank_fg_l[4]=" hppph ";tank_bg_l[4]=" ihhhi ";
    tank_ch_l[5]=" XdXXX ";tank_fg_l[5]=" hXhhh ";tank_bg_l[5]=" iiiii ";
    tank_ch_l[6]="aaaaaaa";tank_fg_l[6]="iiiiiii";tank_bg_l[6]="hhhhhhh";

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
    for(i=0;i<17;i++){
        tank_symbol[i] = i+12;
        gfx_set_symbol(i+12, tank_sprite[i]);
    }
}

void map_draw(map m, int camera_x, int camera_y){
    if(m.map_data == 0)return;
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
        if(x>=0&&y>=0&&x<7&&y<7){
            char** tank_ch;
            char** tank_fg;
            char** tank_bg;
            int dir = 3;
            int player = 1;
            int decal = 0;

            if(player == 0)decal = RED|BRIGHT;
            if(player == 1)decal = BLUE|BRIGHT;
            if(player == 2)decal = GREEN|BRIGHT;
            if(player == 3)decal = RED|GREEN|BRIGHT;
            if(player == 4)decal = BLUE|GREEN|BRIGHT;
            if(player == 5)decal = RED|BLUE|BRIGHT;
            if(player == 6)decal = RED|GREEN|BLUE|BRIGHT;
            if(player == 7)decal = RED|GREEN;

            if(dir == 0){
                tank_ch = tank_ch_u;
                tank_fg = tank_fg_u;
                tank_bg = tank_bg_u;
            }

            if(dir == 1){
                tank_ch = tank_ch_r;
                tank_fg = tank_fg_r;
                tank_bg = tank_bg_r;
            }

            if(dir == 2){
                tank_ch = tank_ch_d;
                tank_fg = tank_fg_d;
                tank_bg = tank_bg_d;
            }

            if(dir == 3){
                tank_ch = tank_ch_l;
                tank_fg = tank_fg_l;
                tank_bg = tank_bg_l;
            }

            int tch = tank_ch[y][x];
            if(tch == ' ')put = ' ';
            else if(tch == 'X')put = dirt_symbol[1];
            else put = tank_symbol[tch - 'a'];

            int tfg = tank_fg[y][x];
            if(tfg == ' ')fg = RED|GREEN|BLUE;
            else if(tfg == 'X')fg = decal;
            else fg = tfg - 'a';

            int tbg = tank_bg[y][x];
            if(tbg == ' ')bg = BLACK;
            else bg = tbg - 'a';
        }
        gfx_put(x, y, put, bg, fg);
    }
}
