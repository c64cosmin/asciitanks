#ifndef _LISTENING_H_
#define _LISTENING_H_

#include "messaging.h"

#define MAX_CONNECTION_NO 8

void listening(char* address, int port);
void get_connections(connection* conn);

#endif //_LISTENING_H_
