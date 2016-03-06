#include "../common/connection.h"

int main(int argn, char** argv){
    if(argn != 3){
        printf("Usage: %s ipaddress port\n", argv[0]);
        return 1;
    }
    int portnumber = atoi(argv[2]);
    char* ipaddress = argv[1];
    SOCKET_HANDLER sh = create_socket(ipaddress, portnumber);
    
    if(sh.socketfd == -1){
        return 1;
    }

    printf("Listening on %s:%i\n", ipaddress, portnumber);

    listen_for_connection(sh);
    return 0;
}
