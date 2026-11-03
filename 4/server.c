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
    while(1){
        struct sockaddr_in client_addr;
        socklen_t client_len=sizeof(client_addr);
        int newsockfd=accept(sockfd,(struct sockaddr*)&client_addr,&client_len);
        if(newsockfd<0){
            perror("accept error");
            exit(1);
        }
        pid_t pid=fork();
        if(pid==0){
            close(sockfd);
            char buffer[4096];
            memset(buffer,0,sizeof(buffer));
            int n=recv(newsockfd,buffer,sizeof(buffer),0);
            if(n<=0){
                perror("recv error");
                close(newsockfd);
                break;
            }
            char method[16],uri[256];
            sscanf(buffer,"%15s %255s",method,uri);
            char filename[256];
            snprintf(filename,sizeof(filename),"%s",uri+1);
            printf("Received filename: %s\n",filename);
            FILE *fp=fopen(filename,"r");
            if(fp==NULL){
                perror("file open error");
                break;
            }
            fseek(fp,0,SEEK_END);
            long long filesize=ftell(fp);
            fseek(fp,0,SEEK_SET);
            char header[256];
            snprintf(header, sizeof(header), 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n"
                "Content-Length: %lld\r\n"
                "Connection: close\r\n"
                "\r\n", filesize);
            printf("Sending Header:\n%s", header);
            send(newsockfd,header,strlen(header),0);
            memset(buffer,0,sizeof(buffer));
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