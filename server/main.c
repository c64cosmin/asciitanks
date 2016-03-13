#include "../common/listening.h"
#include "../common/game/map.h"
#include "../common/game/player.h"
#include "client_state.h"
#include <stdio.h>
#include <unistd.h>

void update_player(int i, map m, connection conn, client_state* state, player* p);

int main(int argn, char** argv){
    if(argn != 3){
        printf("Usage: %s ipaddress port\n", argv[0]);
        return 1;
    }
    char* address = argv[1];
    int port = atoi(argv[2]);

    connection connections[MAX_CONNECTION_NO];
    char msg[2048];

    map game_map = new_map(400,300);

    player players[MAX_CONNECTION_NO];
    client_state clients[MAX_CONNECTION_NO];

    listening(address, port);
    while(1){
        get_connections(connections);
        int i;
        for(i=0;i<MAX_CONNECTION_NO;i++)
            update_player(i, game_map, connections[i], &clients[i], &players[i]);
    }
    return 0;
}

void update_player(int i, map m, connection conn, client_state* state, player* p){
    if(connection_alive(conn)){
        if(state->connection_state == 0){
            state->connection_state = 1;
            state->map_state = 0;
            return;
        }
        else{
            if(state->map_progress == 0){
                state->map_progress = 1;
                state->map_progress_x = 0;
                state->map_progress_y = 0;
                //1 header + 2sizex + 2sizey + 1 zero terminated
                char map_buffer[6];
                map_buffer[5] = 0;
                map_buffer[0] = 0;//sending map size
                
                int sx = m.map_x;
                int sy = m.map_y;

                map_buffer[1] = sx&0xFF;
                map_buffer[2] = (sx>>8)&0xFF;

                map_buffer[3] = sy&0xFF;
                map_buffer[4] = (sy>>8)&0xFF;
                send_string(conn, map_buffer);
                return;
            }
            else{
                if(state->map_state == 0{
                    if(state->map_progress_y >= m.map_y){
                        state->map_state = 1;
                        return;
                    }
                    //1 header + 2posx + 2posy + 8*8 + 1 zero terminated
                    char map_buffer[70];
                    map_buffer[69] = 0;
                    map_buffer[0] = 1;//sending map chunk

                    int x,y;
                    int sx = state->map_progress_x;
                    int sy = state->map_progress_y;

                    map_buffer[1] = sx&0xFF;
                    map_buffer[2] = (sx>>8)&0xFF;

                    map_buffer[3] = sy&0xFF;
                    map_buffer[4] = (sy>>8)&0xFF;

                    for(x=0;x<8;x++)
                    for(y=0;y<8;y++){
                        map_buffer[x+y*8+5] = get_map(m, x + sx, y + sy);
                    }

                    send_string(conn, map_buffer);
                    state->map_progress_x += 8;
                    if(state->map_progress_x >= m.map_x){
                        state->map_progress_x = 0;
                        state->map_progress_y += 8;
                    }
                    return;
                }
                else{
                    //done, can do game logic
                }
        }
    }
    else{
        state->connection_state = 0;
    }
}
