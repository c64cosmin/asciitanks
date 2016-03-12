#include "../common/messaging.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"
#include "kbd.h"

int main(int argn, char** argv){
    kbd_init();
    gfx_init();
    int x=0,y=0;
    while(1){
        usleep(10000);
        int key = kbd_get();
        if(key=='a'||key=='A')x--;
        if(key=='d'||key=='D')x++;
        if(key=='w'||key=='W')y--;
        if(key=='s'||key=='S')y++;
        if(key!=0)
        gfx_put(x,y, key, BLACK, RED); 
        gfx_blit();
    }
    if(argn != 3){
        printf("Usage: %s ipaddress port\n", argv[0]);
        return 1;
    }
    char* address = argv[1];
    int port = atoi(argv[2]);

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
