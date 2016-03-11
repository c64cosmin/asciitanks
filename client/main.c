#include "../common/messaging.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "gfx.h"

int main(int argn, char** argv){
    gfx_init();
    int i,j;
    for(i=0;i<16;i++)
    for(j=0;j<16;j++)
    gfx_put(i+3,j+3, (i*j)%15+'a', i,j);//BLACK, RED);
    gfx_blit();
    return 0;
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
