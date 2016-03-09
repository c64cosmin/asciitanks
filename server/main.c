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

    listening(address, port);
    while(1){
        usleep(1000000);
        get_connections(connections);
        int i;
        for(i=0;i<MAX_CONNECTION_NO;i++){
            if(connections[i]==0)printf("xxx ");
            else printf("%p ", connections[i]);
        }
        printf("\n");
    }
    return 0;
}
