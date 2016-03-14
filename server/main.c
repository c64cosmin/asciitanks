#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "../common/listening.h"
#include "../common/game/map.h"
#include "../common/game/player.h"
#include "client_state.h"

void update_player(int i, map m, connection conn, client_state* state, player* p);
void init_map(map m);
void get_spawn_point(map m, int i, int* x,int* y);

int main(int argn, char** argv){
    if(argn != 3){
        printf("Usage: %s ipaddress port\n", argv[0]);
        return 1;
    }
    char* address = argv[1];
    int port = atoi(argv[2]);

    connection connections[MAX_CONNECTION_NO];
    unsigned char msg[256];

    map game_map = new_map(600,600);
    init_map(game_map);

    player players[MAX_CONNECTION_NO];
    client_state clients[MAX_CONNECTION_NO];

    listening(address, port);
    while(1){
        usleep(10);
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
            state->map_progress = 0;
            return;
        }
        else{
            if(state->map_progress == 0){
                state->map_progress = 1;
                state->map_state = 0;
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
                send_string(conn, map_buffer, 6);
                return;
            }
            else{
                if(state->map_state == 0){
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

                    send_string(conn, map_buffer, 70);
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
    }
    else{
        state->connection_state = 0;
    }
}

int stone_gen(int x_pos, int y_pos){
    double x=x_pos*0.2;
    double y=y_pos*0.2;
    double res=0;
    res += cos(x* 0.365+y* 0.484);
    res += cos(x* 0.257+y* 0.123);
    res += cos(x*-0.245+y* 0.285);
    res += cos(x*-0.524+y* 0.721);
    res += cos(x* 0.437+y*-0.752);
    res += cos(x* 0.135+y*-0.286);
    res += cos(x*-0.834+y*-0.632);
    res += cos(x*-0.372+y*-0.126);
    if(res>3.0)return 1;
    return 0;
}

int grass_gen(int x_pos, int y_pos){
    double x=x_pos*0.3;
    double y=y_pos*0.3;
    double res=0;
    res += cos(x* 0.372+y* 0.158);
    res += cos(x* 0.346+y* 0.821);
    res += cos(x*-0.247+y* 0.466);
    res += cos(x*-0.732+y* 0.157);
    res += cos(x* 0.259+y*-0.168);
    res += cos(x* 0.825+y*-0.189);
    res += cos(x*-0.486+y*-0.822);
    res += cos(x*-0.159+y*-0.168);
    if(res>3.8)return 1;
    return 0;
}

void get_spawn_point(map m, int i, int* x, int* y){
    int margin=30;
    if(i == 0){
        *x = margin;
        *y = m.map_y/2;
    }
    if(i == 1){
        *x = m.map_x-margin;
        *y = m.map_y/2;
    }
    if(i == 2){
        *x = m.map_x/2;
        *y = margin;
    }
    if(i == 3){
        *x = m.map_x/2;
        *y = m.map_y-margin;
    }
    if(i == 4){
        *x = margin;
        *y = margin;
    }
    if(i == 5){
        *x = m.map_x-margin;
        *y = m.map_y-margin;
    }
    if(i == 6){
        *x = m.map_x-margin;
        *y = margin;
    }
    if(i == 7){
        *x = margin;
        *y = m.map_y-margin;
    }
}

int spawn_gen(map m,int x_pos, int y_pos){
    int i;
    for(i=0;i<8;i++){
        int x,y;
        get_spawn_point(m, i, &x, &y);
        int d = (int)sqrt((x_pos-x)*(x_pos-x)+(y_pos-y)*(y_pos-y));
        if(d<10)return 1;
    }
    return 0;
}

void init_map(map m){
    int x,y;
    for(x=0;x<m.map_x;x++)
    for(y=0;y<m.map_y;y++){
        int set = MAP_DIRT;
        if(stone_gen(x,y))set = MAP_STONE;
        if(grass_gen(x,y))set = MAP_GRASS;
        if(spawn_gen(m,x,y))set = MAP_EMPTY;
        set_map(m,x,y,set);
    }
}
