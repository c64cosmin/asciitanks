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

    int connections[MAX_CONNECTION_NO];

    listening(address, port);
    while(1){
        usleep(1000000);
        get_connections(connections);
        int i;
        for(i=0;i<MAX_CONNECTION_NO;i++){
            if(connections[i]==-1)printf("xxx ");
            else printf("%3i ", connections[i]);
        }
        printf("\n");
    }
    return 0;
}
