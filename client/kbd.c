#include <pthread.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define KEYS_NO 256

char key;
pthread_mutex_t kbd_lock;

void* kbd_thread(void* args){
    char* k = (char*)args;
    while(1){
        pthread_mutex_lock(&kbd_lock);
        int kstate = *k;
        pthread_mutex_unlock(&kbd_lock);
        if(kstate!=0){
            usleep(1000);
        }
        else{
            int c = getchar();
            pthread_mutex_lock(&kbd_lock);
            *k = c;
            pthread_mutex_unlock(&kbd_lock);
        }
    }
}

void kbd_init(){
    //most voodoo stuff ever, this shouldn't work
    initscr();
    key=0;

    pthread_mutex_init(&kbd_lock, 0);

    pthread_t thread;
    pthread_create(&thread, 0, kbd_thread, (void*)&key);
}

int kbd_get(){
    int r;
    pthread_mutex_lock(&kbd_lock);
    r=key;
    key=0;
    pthread_mutex_unlock(&kbd_lock);
    return r;
}
