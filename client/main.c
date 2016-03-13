#include "../common/messaging.h"
#include "../common/game/map.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"
#include "kbd.h"

int main(int argn, char** argv){
    kbd_init();
    gfx_init();
    gfx_clear();
    map_draw_init();
    int x = 0;
    int y = 0;
    map game_map = new_map(100,100);
    int i,j;
    for(i=0;i<50;i++)
    for(j=0;j<50;j++){
        set_map(game_map, i,j, MAP_GRASS);
        set_map(game_map, i+50,j, MAP_STONE);
        set_map(game_map, i,j+50, MAP_DIRT);
    }
    int running = 1;
    while(running){
        int key = kbd_get();
        if(key == KBD_ESC)running = 0;
        if(key == 'a')x--;
        if(key == 'd')x++;
        if(key == 'w')y--;
        if(key == 's')y++;

        map_draw(game_map, x, y);
        gfx_blit();
    }
    gfx_clear();
    gfx_blit();
    gfx_deinit();
    return 0;



    if(argn != 4){
        printf("Usage: %s ipaddress port\n", argv[0]);
        return 1;
    }
    char* address = argv[1];
    int port = atoi(argv[2]);
    char* playername = argv[3];

    connection c = new_connection(address, port);
    char msg[1024];
    while(connection_alive(c)){
        usleep(1000000);
        printf("I'm alive\n");
        send_string(c, "synchy");
        recv_string(c, msg);
        printf("serv:%s\n",msg);
    }
    printf("Server has stopped\n");
    return 0;
}
