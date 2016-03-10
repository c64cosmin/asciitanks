#ifndef _MESSAGING_H_
#define _MESSAGING_H_

typedef void* connection;

connection new_connection(char* address, int port);
void send_string(connection conn, char* msg, int size);
void recv_string(connection conn, char* msg, int size);
int connection_alive(connection conn);

#endif //_MESSAGING_H_
