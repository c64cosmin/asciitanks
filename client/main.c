#include "../common/messaging.h"
#include <stdio.h>
#include <unistd.h>

int main(int argn, char** argv){
    if(argn != 3){
        printf("Usage: %s ipaddress port\n", argv[0]);
        return 1;
    }
    char* address = argv[1];
    int port = atoi(argv[2]);

    connection c = new_connection(address, port);
    while(connection_alive(c)){
        usleep(1000000);
        printf("I'm alive\n");
    }
    return 0;
}
