#include "../common/connection.h"
#include <stdio.h>

SOCKET_HANDLE connect_to_ip(char* address, int port){
}

void send_data(SOCKET_HANDLE sh, void* data, int data_size){
}

void* recv_data(SOCKET_HANDLE sh, int* data_size){
}

SOCKET_HANDLE create_socket(char* address, int port){
    SOCKET_HANDLE sh;
    sh.socketfd = socket(AF_INET, SOCK_STREAM, 0);
    
    //if socketfd is -1 we have a error
    if(sh.socketfd == -1){
        printf("ERROR: Cannot create the socket.\n");
        sh.socketfd = -1;
        return sh;
    }

    struct addrinfo addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(adress);
    memset(my_addr.sin_zero, 0, sizeof(my_addr.sin_zero);

    //bind the socket to the requested address
    int errorno = bind(sh.socketfd, &addr, sizeof(addr));

    if(errorno == -1){
        printf("ERROR: Cannot bind the socket for %s:%i\n", address, port);
        sh.socketfd = -1;
        return sh;
    }
    
    return sh;
}

SOCKET_HANDLE listen_for_connection(SOCKET_HANDLE sh){
    SOCKET_HANDLE new_sh;
    int errorno;
    
    errorno = listen(sh.socketfd, 10);

    if(errorno == -1){
        printf("ERROR: Cannot listen on the socket\n");
        new_sh.socketfd = -1;
        return new_sh;
    }

    struct addrinfo addr;
    int addr_size = sizeof(addr);
    new_sh.socketfd = accept(sh, &addr, &addr_size);

    return new_sh;
}
