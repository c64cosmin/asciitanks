#ifndef _GFX_H_
#define _GFX_H_

#define BLACK 0
#define RED 1
#define GREEN 2
#define BLUE 4
#define BRIGHT 8
#define PLAIN_COLOR 7

//initializes the ascii graphics library
//if x and y are 0 then it will initialize the whole console screen
void gfx_init();
void gfx_deinit();
void gfx_clear();
void gfx_blit();
void gfx_put(int x, int y, int c, int bg_color, int fg_color);

void gfx_set_symbol(int c, char* s);
#endif //_GFX_H_
