#ifndef _CLIENT_STATE_H_
#define _CLIENT_STATE_H_

typedef struct _client_state{
    int connection_state;
    int map_state;
    int map_progress;
    int map_progress_x;
    int map_progress_y;
};

#endif //_CLIENT_STATE_H_
