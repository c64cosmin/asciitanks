#include <stdio.h>
#include "../common/messaging.h"

int main(int argn, char** argv){
    if(argn != 3){
        printf("Usage: %s ipaddress port\n", argv[0]);
        return 1;
    }
    char* address = argv[1];
    int port = atoi(argv[2]);

    connection c = new_connection(address, port);
    return 0;
}
