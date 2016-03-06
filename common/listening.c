#include "common/listening.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

void listen(char* address, int port){
    int socketfd;
    
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    
    //if socketfd is -1 we have a error
    if(socketfd == -1){
        printf("ERROR: Cannot create the socket.\n");
        return;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(adress);
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero);

    //bind the socket to the requested address
    int errorno = bind(socketfd, &addr, sizeof(addr));

    if(errorno == -1){
        printf("ERROR: Cannot bind the socket for %s:%i\n", address, port);
        return;
    }

    socklen_t addr_size;
    sockaddr_storage new_addr;
    int newsocketfd;

    while(true){
        listen(socketfd, 10);
        addr_size = sizeof(new_addr);
        newsocketfd = accept(socketfd, (struct sockaddr*)&new_addr, &addr_size);

        char *new_ip = inet_ntoa(((sockaddr_in)new_addr).sin_addr);
        printf("%s has connected\n", new_ip);
    }
}
