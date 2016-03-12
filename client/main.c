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
    map_draw_init();
    map game_map = new_map(100,100);
    map_draw(game_map);
    gfx_blit();
    gfx_deinit();
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
