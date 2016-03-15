#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/messaging.h"
#include "../common/game/map.h"
#include "../common/game/player.h"
#include "gfx.h"
#include "kbd.h"

#define CHUNK_SIZE 60

void update_player(map* m, connection conn, player* p, int* id);

unsigned char msg[4096];
int msg_len;

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
    if(strlen(playername)>10)
        playername[10]=0;

    connection c = new_connection(address, port);
    char running = 1;
    char sent_name = 0;
    map game_map;
    //init players
    int my_id=-1;
    player players[8];
    int i;
    for(i=0;i<8;i++){
        players[i].id = i;
        players[i].online=0;
    }
    game_map.map_data = 0;

    int xc=0,yc=0;

    while(connection_alive(c) && running){
        update_player(&game_map, c, players, &my_id); 
        char key = kbd_get();
        if(key == KBD_ESC)running = 0;
        if(key == 'a' || key == 'd' || key == 'w' || key == 's' || key == ' '){
            char buffer[2];
            buffer[0] = 1;//send key
            buffer[1] = key;
            send_string(c, buffer, 2);
        }
        if(my_id!=-1){
            if(sent_name == 0){
                sent_name = 1;
                char buffer[11];
                buffer[0] = 0;//send my name
                int j;
                for(j=0;j<10;j++)
                    buffer[j+1] = ' ';
                strcpy(&buffer[1], playername);
                send_string(c, buffer, 11);
            }
            map_draw(game_map, players, my_id);
        }
        gfx_blit();
    }
    gfx_clear();
    gfx_blit();
    gfx_deinit();
    if(running)
        printf("Server stopped\n");
    return 0;
}

void update_player(map* m, connection conn, player* p, int* id){
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
        for(x=0;x<CHUNK_SIZE;x++)
        for(y=0;y<CHUNK_SIZE;y++)
            set_map(*m, x + pos_x, y + pos_y, msg[x+y*CHUNK_SIZE+5]);
    }
    if(msg[0] == 2){//receive my id
        *id = msg[1];
    }
    if(msg[0] == 3){//receive game state
        memcpy((void*)p, (void*)&msg[1], sizeof(player)*8);
    }
}
