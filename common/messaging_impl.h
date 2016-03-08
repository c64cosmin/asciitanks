#ifndef _MESSAGING_IMPL_
#define _MESSAGING_IMPL_

#include <pthread.h>

struct _connection{
    int socket_fd;
    pthread_mutex_t lock;
};

void create_messaging_thread(struct _connection* conn);
void* messaging_thread(void* args);

#endif //_MESSAGING_IMPL_
