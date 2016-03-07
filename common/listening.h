#ifndef _LISTENING_H_
#define _LISTENING_H_

#define MAX_CONNECTION_NO 8

void listening(char* address, int port);
void get_connections(int* conn);

#endif //_LISTENING_H_
