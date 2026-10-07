#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

void signal_handler(int);

int main(void){
    signal(SIGINT,signal_handler);
    
    while(1){

    }
}

void signal_handler(int signal){
    puts("中断");
    exit(1);
}