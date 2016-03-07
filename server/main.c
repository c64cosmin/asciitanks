#include "../common/listening.h"
#include <pthread.h>
#include <stdio.h>

int main(int argn, char** argv){
    if(argn != 3){
        printf("Usage: %s ipaddress port\n", argv[0]);
        return 1;
    }
    char* address = argv[1];
    int port = atoi(argv[2]);
    
    listening(address, port);
    while(1);
    return 0;
}
