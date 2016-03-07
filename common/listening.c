#include "listening.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

struct listening_thread_args{
    char* address;
    int port;
};

void* listening_impl(void* args){
    char* address = ((struct listening_thread_args*)args)->address;
    int port = ((struct listening_thread_args*)args)->port;
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
    addr.sin_addr.s_addr = inet_addr(address);
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

    //bind the socket to the requested address
    int errorno = bind(socketfd, (struct sockaddr*)&addr, sizeof(addr));

    if(errorno == -1){
        printf("ERROR: Cannot bind the socket for %s:%i\n", address, port);
        return;
    }

    socklen_t addr_size;
    struct sockaddr_in new_addr;
    int newsocketfd;

    printf("Started listening on %s:%i\n", address, port);

    while(1){
        listen(socketfd, 10);
        addr_size = sizeof(new_addr);
        newsocketfd = accept(socketfd, (struct sockaddr*)&new_addr, &addr_size);
        
        //byte by byte printing to get the IPv4 address of the conectee
        printf("%u.%u.%u.%u has connected\n",
               ((unsigned char*)&new_addr.sin_addr.s_addr)[0],
               ((unsigned char*)&new_addr.sin_addr.s_addr)[1],
               ((unsigned char*)&new_addr.sin_addr.s_addr)[2],
               ((unsigned char*)&new_addr.sin_addr.s_addr)[3]);
    }
}

void listening(char* address, int port){
    struct listening_thread_args args;
    args.address = address;
    args.port = port;
    
    pthread_t thread_id;
    int errorno = pthread_create(&thread_id, 0, listening_impl, (void*)&args);
    if(errorno){
        printf("ERROR: Cannot create a new thread\n");
    }
}
