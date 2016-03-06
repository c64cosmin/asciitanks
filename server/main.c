#include "../common/listening.h"
#include <stdio.h>

int main(int argn, char** argv){
    if(argn != 3){
        printf("Usage: %s ipaddress port\n", argv[0]);
        return 1;
    }
    int portnumber = atoi(argv[2]);
    char* ipaddress = argv[1];
    
    listening(ipaddress, portnumber);
    return 0;
}
