#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct _player{
    char id;
    char name[10];
    int pos_x;
    int pos_y;
    char direction;
    char bullet_is;
    int bullet_pos_x;
    int bullet_pos_y;
    char bullet_direction;
} player;

#endif //_PLAYER_H_
