#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../common/messaging.h"
#include "../common/game/map.h"
#include "../common/game/player.h"
#include "gfx.h"
#include "kbd.h"

void update_player(map* m, connection conn, player* p);

int main(int argn, char** argv){
    if(argn != 4){
        printf("Usage: %s ipaddress port playername\n", argv[0]);
        return 1;
    }
    kbd_init();
    gfx_init();
    gfx_clear();

    map_draw_init();

    char* address = argv[1];
    int port = atoi(argv[2]);
    char* playername = argv[3];

    connection c = new_connection(address, port);
    char running = 1;
    map game_map;
    player p;
    game_map.map_data = 0;

    while(connection_alive(c) && running){
        usleep(1000);
        update_player(&game_map, c, &p); 
        char key = kbd_get();
        if(key == KBD_ESC)running = 0;
        map_draw(game_map, 0, 0);
        gfx_blit();
    }
    gfx_clear();
    gfx_blit();
    gfx_deinit();
    if(!running)
        printf("Server stopped\n");
    return 0;
}

void update_player(map* m, connection conn, player* p){
    unsigned char msg[256];
    int msg_len;
    recv_string(conn, msg, &msg_len);
    if(msg_len==0)return;
    if(msg[0] == 0){//receive map size
        int size_x = msg[1]|(msg[2]<<8);
        int size_y = msg[3]|(msg[4]<<8);
        
        *m = new_map(size_x, size_y);
    }
    if(msg[0] == 1){//receive map chunk
        int pos_x = msg[1]|(msg[2]<<8);
        int pos_y = msg[3]|(msg[4]<<8);
        
        int x,y;
        for(x=0;x<8;x++)
        for(y=0;y<8;y++)
            set_map(*m, x + pos_x, y + pos_y, msg[x+y*8+5]);
    }
    
}
