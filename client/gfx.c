#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"

#define BUFFER_SIZE 10000
#define XY(x,y) (y*console_x+x)
#define GET_BG(c) (c&0xF)
#define GET_FG(c) (c>>4&0xF)

int console_x;
int console_y;
char* color_buffer;
char* gfx_buffer;

void gfx_init(){
    struct winsize sz;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &sz);
    
    console_x = sz.ws_col;
    console_y = sz.ws_row;
    
    color_buffer = (char*)malloc(sizeof(char)*console_x*console_y);
    gfx_buffer = (char*)malloc(sizeof(char)*console_x*console_y);
    if(color_buffer == 0 || gfx_buffer == 0){
        printf("ERROR:Cannot initialize gfx\n");
        console_x=0;
        console_y=0;
    }
    gfx_clear();
}

void gfx_clear(){
    if(console_x==0||console_y==0)return;
    int x;
    int y;
    for(x=0;x<console_x;x++)
    for(y=0;y<console_y;y++){
        color_buffer[XY(x,y)]=BLACK;
        gfx_buffer[XY(x,y)]=' ';
    }
}

void gfx_blit(){
    if(console_x==0||console_y==0)return;
    int x,y;
    printf("\033[0;0H");
    for(y=0;y<console_y;y++){
        for(x=0;x<console_x;x++){
            int c = color_buffer[XY(x,y)];
            int bg = (GET_BG(c)&PLAIN_COLOR)+((GET_BG(c)&BRIGHT)==BRIGHT?100:40);
            int fg = (GET_FG(c)&PLAIN_COLOR)+((GET_FG(c)&BRIGHT)==BRIGHT? 90:30);
            printf("\033[%i;%im%c", fg, bg, gfx_buffer[XY(x,y)]);
        }
    }
}

void gfx_put(int x, int y, char c, int bg_color, int fg_color){
    if(x<0||x>=console_x||y<0||y>=console_y)return;
    color_buffer[XY(x,y)] = bg_color&0xF | (fg_color&0xF)<<4;
    gfx_buffer[XY(x,y)] = c;
}
