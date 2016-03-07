#include "listening.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

int* connections;
pthread_mutex_t connection_list_lock;

struct listening_thread_args{
    char* address;
    int port;
    int* connections;
};

void get_connections(int* conn){
    pthread_mutex_lock(&connection_list_lock);
    memcpy(conn, connections, sizeof(int) * MAX_CONNECTION_NO);
    pthread_mutex_unlock(&connection_list_lock);
}

void* listening_impl(void* args){
    struct listening_thread_args arg = *(struct listening_thread_args*)args;
    char* address = arg.address;
    int port = arg.port;
    int* connections = arg.connections;
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

    //initialize the connection list
    int i;
    for(i=0;i<MAX_CONNECTION_NO;i++){
        connections[i] = -1;
    }

    errorno = listen(socketfd, 10);
    if(errorno == -1){
        printf("ERROR: Cannot start listening on %s:%i\n", address, port);
        return;
    }
    printf("Started listening on %s:%i\n", address, port);

    while(1){
        addr_size = sizeof(new_addr);
        newsocketfd = accept(socketfd, (struct sockaddr*)&new_addr, &addr_size);
        
        i=0;
        pthread_mutex_lock(&connection_list_lock);
        //find a empty connection slot
        while(connections[i]!=-1 && i<MAX_CONNECTION_NO){
            i++;
        }
        //did we find any empty slot?
        if(i!=MAX_CONNECTION_NO){
            //byte by byte printing to get the IPv4 address of the conectee
            printf("%u.%u.%u.%u has connected\n",
                   ((unsigned char*)&new_addr.sin_addr.s_addr)[0],
                   ((unsigned char*)&new_addr.sin_addr.s_addr)[1],
                   ((unsigned char*)&new_addr.sin_addr.s_addr)[2],
                   ((unsigned char*)&new_addr.sin_addr.s_addr)[3]);
            connections[i] = newsocketfd;
        }
        else{
            //refuse connection
            close(newsocketfd);
        }
        pthread_mutex_unlock(&connection_list_lock);
    }
}

void listening(char* address, int port){
    connections = (int*)malloc(sizeof(int) * MAX_CONNECTION_NO);
    struct listening_thread_args *args = (struct listening_thread_args*)malloc(sizeof(struct listening_thread_args));
    args->address = address;
    args->port = port;
    args->connections = connections;
    
    pthread_t thread_id;
    int errorno = pthread_create(&thread_id, 0, listening_impl, (void*)args);
    if(errorno){
        printf("ERROR: Cannot create a new thread\n");
    }
}
