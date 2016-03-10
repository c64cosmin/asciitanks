#include "messaging.h"
#include "messaging_impl.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

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
    //initialize the connection
    conn->alive = 1;
    int i;
    for(i=0;i<SOCK_BUFFER_SIZE;i++){
        conn->recv_buffer[i]=0;
    }
    //create a new thread for the connection
    create_messaging_thread(conn);

    return (connection)conn;
}

void send_string(connection conn, char* msg){
    if(!connection_alive(conn))return;
    char buff[SOCK_BUFFER_SIZE];
    char* buffer = buff;
    int len = strlen(msg);
    //we need two header bytes for the size of message
    if(len == 0 || len + 2 >= SOCK_BUFFER_SIZE)return;
    buffer[0] = (len>>0) & 0xFF;
    buffer[1] = (len>>8) & 0xFF;
    strcpy(&buffer[2], msg);
    struct _connection* c = (struct _connection*)(void*)conn;
    //remaining bytes to send
    int r = len + 2;
    int s;
    while(r>0){
        s = send(c->socket_fd, (void*)buffer, r, 0);
        //we sent s bytes, so we have r remaining
        r-=s;
        //advance in the string
        buffer+=s;
    }
}

void recv_string(connection conn, char* msg){
    if(!connection_alive(conn))return;
    struct _connection* c = (struct _connection*)(void*)conn;
    int no_msg = 0;
    pthread_mutex_lock(&c->lock);
    if(strlen(c->recv_buffer) == 0){
        no_msg = 1;
        msg[0]=0;
    }
    if(no_msg == 0){
        strcpy(msg, c->recv_buffer);
        //empty the buffer
        c->recv_buffer[0]=0;
    }
    pthread_mutex_unlock(&c->lock);
}

void create_messaging_thread(struct _connection* conn){
    //initialize a new mutex for the connection
    pthread_mutex_init(&conn->lock, 0);

    pthread_t thread_id;
    int errorno = pthread_create(&thread_id, 0, messaging_thread, (void*)conn);
    if(errorno){
        printf("ERROR: Cannot create a new thread\n");
    }
}

int connection_alive(connection conn){
    if(conn==0)return 0;
    struct _connection* c = (struct _connection*)(void*)conn;
    int status = 0;
    pthread_mutex_lock(&c->lock);
    status = c->alive;
    pthread_mutex_unlock(&c->lock);
    return status;
}

void close_connection(struct _connection* c){
    pthread_mutex_lock(&c->lock);
    c->alive = 0;
    close(c->socket_fd);
    pthread_mutex_unlock(&c->lock);
}

void* messaging_thread(void* args){
    struct _connection* arg = (struct _connection*) args;
    int socket_fd = arg->socket_fd;

    int connection_open = 1;
    char recv_str[SOCK_BUFFER_SIZE];
    char recv_size[2];
    //fast initialize the buffer
    recv_str[0]=0;
    while(connection_open){
        usleep(1000);
        pthread_mutex_lock(&arg->lock);
        int recv_buf_len = strlen(arg->recv_buffer);
        pthread_mutex_unlock(&arg->lock);
        if(recv_buf_len == 0){
            int r = 2;
            int recv_nr;
            char* precv = recv_size;
            while(r>0){
                recv_nr = recv(socket_fd, (void*)precv, r, 0);
                if((recv_nr == 0 && r>0) || (recv_nr == -1)){
                    close_connection(arg);
                    connection_open = 0;
                    break;
                }
                r-=recv_nr;
                precv+=recv_nr;
            }
            r = ((unsigned int)(recv_size[0])<<0)+((unsigned int)(recv_size[1])<<8);
            precv = recv_str;
            while(r>0){
                recv_nr = recv(socket_fd, (void*)precv, r, 0);
                if((recv_nr == 0 && r>0) || (recv_nr == -1)){
                    close_connection(arg);
                    connection_open = 0;
                    break;
                }
                r-=recv_nr;
                precv+=recv_nr;
            }
            //copy the received message
            pthread_mutex_lock(&arg->lock);
            strcpy(arg->recv_buffer, recv_str);
            pthread_mutex_unlock(&arg->lock);
            //empty our receiving string
            recv_str[0]=0;
        }
    }
}
