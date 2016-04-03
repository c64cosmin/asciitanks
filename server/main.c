#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "../common/listening.h"
#include "../common/game/map.h"
#include "../common/game/player.h"
#include "client_state.h"

#define CHUNK_SIZE 60

void update_player(int i, map m, connection conn, client_state* state, player* p);
void init_map(map m);
void get_spawn_point(map m, int i, int* x,int* y);
void game_logic(int i, map m, player* p, connection conn);
char test_collision(int i, player* p, map m, int move_x, int move_y);

unsigned char msg[4096];
int msg_len;
void bullet_update(int i, player* p, map m);

int main(int argn, char** argv){
    if(argn != 3){
        printf("Usage: %s ipaddress port\n", argv[0]);
        return 1;
    }
    char* address = argv[1];
    int port = atoi(argv[2]);

    connection connections[MAX_CONNECTION_NO];

    map game_map = new_map(100,100);
    init_map(game_map);

    player players[MAX_CONNECTION_NO];
    client_state clients[MAX_CONNECTION_NO];

    //init players
    int i;
    for(i=0;i<8;i++){
        players[i].id = i;
        players[i].online = 0;
        players[i].bullet_is = 0;
    }

    listening(address, port);
    while(1){
        usleep(70000);
        get_connections(connections);
        for(i=0;i<MAX_CONNECTION_NO;i++){
            bullet_update(i, players, game_map);
            update_player(i, game_map, connections[i], &clients[i], players);
            bullet_update(i, players, game_map);
        }
    }
    return 0;
}

void bullet_update(int i, player* players, map m){
    player* p = &players[i];

    if(p->bullet_is == 0)
        return;

    int move_x;
    int move_y;
    if(p->bullet_direction == 0){
        move_x = 0;
        move_y = -1;
    }
    if(p->bullet_direction == 1){
        move_x = 1;
        move_y = 0;
    }
    if(p->bullet_direction == 2){
        move_x = 0;
        move_y = 1;
    }
    if(p->bullet_direction == 3){
        move_x = -1;
        move_y = 0;
    }

    if(get_map(m, p->bullet_pos_x + move_x, p->bullet_pos_y + move_y) != MAP_STONE){
        p->bullet_pos_x += move_x;
        p->bullet_pos_y += move_y;
    }
    else{
        p->bullet_is = 0;
    }

    int j;
    for(j=0;j<8;j++)
    if(i!=j){
        int px = players[j].pos_x;
        int py = players[j].pos_y;
        int x = p->bullet_pos_x;
        int y = p->bullet_pos_y;
        if(x >= px-3 && x <= px+3 && y >= py-3 && y <= py+3)
            players[j].online = 0;
    }
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
                state->id_state = 0;
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
                    //60*60 = 3600
                    //1 header + 2posx + 2posy + 3600 + 1 zero terminated
                    char map_buffer[3606];
                    map_buffer[3605] = 0;
                    map_buffer[0] = 1;//sending map chunk

                    int x,y;
                    int sx = state->map_progress_x;
                    int sy = state->map_progress_y;

                    map_buffer[1] = sx&0xFF;
                    map_buffer[2] = (sx>>8)&0xFF;

                    map_buffer[3] = sy&0xFF;
                    map_buffer[4] = (sy>>8)&0xFF;

                    for(x=0;x<CHUNK_SIZE;x++)
                    for(y=0;y<CHUNK_SIZE;y++){
                        map_buffer[x+y*CHUNK_SIZE+5] = get_map(m, x + sx, y + sy);
                    }

                    send_string(conn, map_buffer, 3606);

                    state->map_progress_x += CHUNK_SIZE;
                    if(state->map_progress_x >= m.map_x){
                        state->map_progress_x = 0;
                        state->map_progress_y += CHUNK_SIZE;
                    }
                    return;
                }
                else{
                    if(state->id_state == 0){
                        state->id_state = 1;

                        char buffer[3];
                        buffer[0] = 2;//sending player id
                        buffer[1] = i;
                        buffer[2] = 0;
                        send_string(conn, buffer, 3);

                        //init player state
                        p[i].id = i;
                        p[i].online = 1;
                        get_spawn_point(m, i, &p[i].pos_x, &p[i].pos_y);
                        p[i].direction = 0;
                        p[i].bullet_is=0;
                    }
                    else{
                        game_logic(i, m, p, conn);
                    }
                }
            }
        }
    }
    else{
        state->connection_state = 0;
        p[i].online = 0;
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
        if(d<13)return 1;
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
        //if(spawn_gen(m,x,y))set = MAP_EMPTY;
        set_map(m,x,y,set);
    }
}

void put_bullet(player* p){
    p->bullet_is = 1;
    p->bullet_direction = p->direction;
    if(p->direction == 0){
        p->bullet_pos_x = p->pos_x;
        p->bullet_pos_y = p->pos_y - 3;
    }
    if(p->direction == 1){
        p->bullet_pos_x = p->pos_x + 3;
        p->bullet_pos_y = p->pos_y;
    }
    if(p->direction == 2){
        p->bullet_pos_x = p->pos_x;
        p->bullet_pos_y = p->pos_y + 3;
    }
    if(p->direction == 3){
        p->bullet_pos_x = p->pos_x - 3;
        p->bullet_pos_y = p->pos_y;
    }
}

void game_logic(int i, map m, player* p, connection conn){
    recv_string(conn, msg, &msg_len);
    if(msg_len > 0){
        if(msg[0] == 0){//receive player name
            memcpy(p[i].name, &msg[1], 10);
        }
        if(msg[0] == 1){//receive player key press
            char key = msg[1];
            if(msg[1] == 'w'){
                if(!test_collision(i, p, m, 0, -1)){
                    p[i].pos_y--;
                }
                p[i].direction = 0;
            }
            if(msg[1] == 's'){
                if(!test_collision(i, p, m, 0,  1)){
                    p[i].pos_y++;
                }
                p[i].direction = 2;
            }
            if(msg[1] == 'a'){
                if(!test_collision(i, p, m, -1, 0)){
                    p[i].pos_x--;
                }
                p[i].direction = 3;
            }
            if(msg[1] == 'd'){
                if(!test_collision(i, p, m,  1, 0)){
                    p[i].pos_x++;
                }
                p[i].direction = 1;
            }
            if(msg[1] == ' '){
                if(p[i].online && p[i].bullet_is == 0){
                    put_bullet(&p[i]);
                }
            }
        }
    }
    //sizeof(player)*8 = 36 * 8 = 288
    char state_buffer[289];
    state_buffer[0] = 3;//send game state
    int it;
    memcpy((void*)&state_buffer[1], (void*)p, sizeof(player)*8); 
    send_string(conn, state_buffer, 289);
}

char two_player_collision(int i, int j, player* p, int move_x, int move_y){
    int px0_0 = p[i].pos_x-3 + move_x;
    int py0_0 = p[i].pos_y-3 + move_y;
    int px0_1 = p[i].pos_x+3 + move_x;
    int py0_1 = p[i].pos_y+3 + move_y;

    int px1_0 = p[j].pos_x-3;
    int py1_0 = p[j].pos_y-3;
    int px1_1 = p[j].pos_x+3;
    int py1_1 = p[j].pos_y+3;

    if(px0_0>=px1_0 && px0_0<=px1_1 &&
       py0_0>=py1_0 && py0_0<=py1_1)return 1;

    if(px0_1>=px1_0 && px0_1<=px1_1 &&
       py0_0>=py1_0 && py0_0<=py1_1)return 1;

    if(px0_0>=px1_0 && px0_0<=px1_1 &&
       py0_1>=py1_0 && py0_1<=py1_1)return 1;

    if(px0_1>=px1_0 && px0_1<=px1_1 &&
       py0_1>=py1_0 && py0_1<=py1_1)return 1;

    return 0;
}

char map_collision(int i, player* p, map m, int move_x, int move_y){
    player pp = p[i];
    int it;
    if(move_x == 1){
        for(it=0;it<7;it++){
            int x = pp.pos_x + 4;
            int y = pp.pos_y - 3 + it;
            if(get_map(m, x, y) == MAP_STONE)
                return 1;
        }
    }
    if(move_x == -1){
        for(it=0;it<7;it++){
            int x = pp.pos_x - 4;
            int y = pp.pos_y - 3 + it;
            if(get_map(m, x, y) == MAP_STONE)
                return 1;
        }
    }
    if(move_y == 1){
        for(it=0;it<7;it++){
            int x = pp.pos_x - 3 + it;
            int y = pp.pos_y + 4;
            if(get_map(m, x, y) == MAP_STONE)
                return 1;
        }
    }
    if(move_y == -1){
        for(it=0;it<7;it++){
            int x = pp.pos_x - 3 + it;
            int y = pp.pos_y - 4;
            if(get_map(m, x, y) == MAP_STONE)
                return 1;
        }
    }
    return 0;
}

char test_collision(int i, player* p, map m, int move_x, int move_y){
    int j;
    
    for(j=0;j<8;j++){
        if(i!=j)
            if(two_player_collision(i, j, p, move_x, move_y))
                return 1;
        if(map_collision(i, p, m, move_x, move_y))
            return 1;
    }
    return 0;
}
