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
        perror("ERROR opening socket");
        exit(1);
    }
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(PORT);
    if(bind(sockfd,(struct sockaddr*)&server,sizeof(server))<0){
        perror("ERROR on binding");
        exit(1);
    }
    if(listen(sockfd,5)<0){
        perror("ERROR on listen");
        exit(1);
    }
    struct sockaddr_in client;
    socklen_t client_len=sizeof(client);
    int newsockfd=accept(sockfd,(struct sockaddr*)&client,&client_len);
    if(newsockfd<0){
        perror("ERROR on accept");
        exit(1);
    }
    FILE *fp=fopen("file.txt","r");
    if(fp==NULL){
        perror("ERROR opening file");
        exit(1);
    }
    char buffer[1024];
    while(fgets(buffer,sizeof(buffer),fp)!=NULL){
        send(newsockfd,buffer,strlen(buffer-1),0);
    }
    fclose(fp);
    close(sockfd);
    close(newsockfd);
    shutdown(sockfd, SHUT_RDWR);
    return 0;
}