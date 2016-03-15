#ifndef _MAP_DRAW_H_
#define _MAP_DRAW_H_

#include "../common/game/map.h"
#include "../common/game/player.h"

#define MAP_SCREEN_X 100
#define MAP_SCREEN_Y 50

void map_draw(map m, player* players, int id);

#endif //_MAP_DRAW_H_
