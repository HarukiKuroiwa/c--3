#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include <curses.h>
//-lcurses cursesをリンク 

void signal_handler(int);

int main(void){
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask,SIGINT);
    sigaddset(&sa.sa_mask,SIGFPE);
    sa.sa_handler=signal_handler;
    sa.sa_flags=0;
    sigaction(SIGINT,&sa,NULL);

    initscr();
    int x=2,y=1;
    while(1){
        int z=x/y;
        if(getch()==32){
            y=0;
        }
        sleep(1);
    }
}

void signal_handler(int signal){
    printf("a signalcaught\n");
    exit(1);
}