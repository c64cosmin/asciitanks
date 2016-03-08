#include "messaging.h"
#include "messaging_impl.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

connection new_connection(char* address, int port){
    int socketfd;
    
    //create a new socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd == -1){
        printf("ERROR: Could not create a new socket\n");
        return 0;
    }
    
    //fill in the address that we want to connect to
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address);
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

    //connect to the server
    int errorno = connect(socketfd, (struct sockaddr*)&addr, sizeof(addr));

    if(errorno == -1){
        printf("ERROR: Cannot connect to server %s:%i\n", address, port);
        return 0;
    }

    //create a new connection object
    struct _connection* conn=(struct _connection*)malloc(sizeof(struct _connection));
    //copy the socket descriptor
    conn->socket_fd = socketfd;
    //initialize a new mutex for the connection
    pthread_mutex_init(&conn->lock, 0);
    //create a new thread for the connection
    create_messaging_thread(conn);

    return (connection)conn;
}

void send_string(connection conn, char* msg, int size){
}

void recv_string(connection conn, char* msg, int size){
}

void create_messaging_thread(struct _connection* conn){
}

void* messaging_thread(void* args){
}
