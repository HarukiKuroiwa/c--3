#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#define PORT 8000

int main(int argc,char *argv[]){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("ERROR opening socket");
        exit(1);
    }
    struct hostent *server_host=gethostbyname("localhost");
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    memcpy(&server.sin_addr.s_addr,server_host->h_addr,server_host->h_length);
    server.sin_port=htons(PORT);
    if(connect(sockfd,(struct sockaddr*)&server,sizeof(server))<0){
        perror("ERROR connecting");
        exit(1);
    }
    send(sockfd,argv[1],strlen(argv[1]),0);
    char buffer[4096];
    memset(buffer,0,sizeof(buffer));
    while(recv(sockfd,buffer,sizeof(buffer),0)>0){
        printf("%s",buffer);
        sleep(0.0001);
    }
    close(sockfd);
    return 0;
}