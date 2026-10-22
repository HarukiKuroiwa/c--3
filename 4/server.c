#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    int sockfd;
    if(sockfd=socket(AF_INET,SOCK_STREAM,0)<0){
        perror("socket error");
        exit(1);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(8080);
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
    int newsockfd=accept(sockfd,(struct sockaddr*)&client_addr,&client_len);
    if(newsockfd<0){
        perror("accept error");
        exit(1);
    }
    char buffer[1024];
    while(1){
        memset(buffer,0,sizeof(buffer));
        int n=recv(newsockfd,buffer,sizeof(buffer),0);
        if(n<=0){
            perror("recv error");
            break;
        }
        FILE *fp;
        if(fp=open(buffer,"r")==NULL){
            perror("file open error");
            break;
        }
        while(fgets(buffer,sizeof(buffer),fp)!=NULL){
            send(newsockfd,buffer,strlen(buffer),0);
        }
        fclose(fp);
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}