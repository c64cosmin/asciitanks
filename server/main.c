#include "../common/listening.h"
#include <stdio.h>
#include <unistd.h>

int main(int argn, char** argv){
    if(argn != 3){
        printf("Usage: %s ipaddress port\n", argv[0]);
        return 1;
    }
    char* address = argv[1];
    int port = atoi(argv[2]);

    connection connections[MAX_CONNECTION_NO];
    char msg[2048];

    listening(address, port);
    while(1){
        usleep(1000000);
        get_connections(connections);
        int i;
        for(i=0;i<MAX_CONNECTION_NO;i++){
            if(connection_alive(connections[i])){
                recv_string(connections[i], msg);
                printf("%i:%s\n",i,msg);
                send_string(connections[i], "jaj");
            }
        }
    }
    return 0;
}
