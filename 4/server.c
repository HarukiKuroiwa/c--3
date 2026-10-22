#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8000

int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("socket error");
        exit(1);
    }
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&(int){1},sizeof(int));
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(PORT);
    server_addr.sin_addr.s_addr=INADDR_ANY;
    if(bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        perror("bind error");
        exit(1);
    }
    if(listen(sockfd,5)<0){
        perror("listen error");
        exit(1);
    }
    struct sockaddr_in client_addr;
    socklen_t client_len=sizeof(client_addr);
    while(1){
        int newsockfd=accept(sockfd,(struct sockaddr*)&client_addr,&client_len);
        pid_t pid=fork();
        if(newsockfd<0){
            perror("accept error");
            exit(1);
        }
        if(pid==0){
            close(sockfd);
            char buffer[4096];
            memset(buffer,0,sizeof(buffer));
            int n=recv(newsockfd,buffer,sizeof(buffer),0);
            if(n<=0){
                perror("recv error");
                break;
            }
            printf("Received filename: %s\n",buffer);
            FILE *fp=fopen(buffer,"r");
            if(fp==NULL){
                perror("file open error");
                break;
            }
            while(fgets(buffer,sizeof(buffer),fp)!=NULL){
                send(newsockfd,buffer,strlen(buffer),0);
            }
            close(newsockfd);
            fclose(fp);
            break;
        }else{
            close(newsockfd);
        }
    }
    return 0;
}