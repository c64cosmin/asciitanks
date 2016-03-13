#ifndef _MESSAGING_IMPL_
#define _MESSAGING_IMPL_

#include <pthread.h>
#define SOCK_BUFFER_SIZE 4096

struct _connection{
    int socket_fd;
    pthread_mutex_t lock;
    int alive;
    unsigned char recv_buffer[SOCK_BUFFER_SIZE];
    char recv_buffer_len;
};

void create_messaging_thread(struct _connection* conn);
void* messaging_thread(void* args);

#endif //_MESSAGING_IMPL_
