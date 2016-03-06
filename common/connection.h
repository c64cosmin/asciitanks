#include <sys/types.h>
#include <sys/socket.h>

typedef struct _SOCKET_HANDLE{
    int socketfd;
} SOCKET_HANDLE;

//connects to an IP address and a port and returns a handler for the connection
SOCKET_HANDLE connect_to_ip(char* address, int port);

//sends data_size bytes from the pointer data to the socket handle
void send_data(SOCKET_HANDLE sh, void* data, int data_size);

//receives data from the socket handle into the pointer to data
//if there is no data to receive it will return 0
void* recv_data(SOCKET_HANDLE sh, int* data_size);

//creates a server socket
SOCKET_HANDLE create_socket(char* address, int port);

//makes the server socket listen for incomming connections
//returns a new connection when some one connected
SOCKET_HANDLE listen_for_connection(SOCKET_HANDLE sh);
