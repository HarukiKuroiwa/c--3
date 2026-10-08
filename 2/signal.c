#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include <curses.h>
//-lcurses cursesをリンク 
void signal_handler(int);

int main(void){
    signal(SIGFPE,signal_handler);

    initscr();
    int x=2,y=1;
    while(1){
        int z=x/y;
        if(getch()==32){
            y=0;
        }
    }
}

void signal_handler(int signal){
    puts("0除算");
    exit(1);
}