#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct _bullet{
    int pos_x;
    int pos_y;
    int direction;
} bullet;

typedef struct _player{
    int pos_x;
    int pos_y;
    int direction;
    bullet bullet[3];
    int bullet_no;
} player;

#endif //_PLAYER_H_
