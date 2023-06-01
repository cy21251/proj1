#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(){
  int new_sockfd;
  struct sockaddr_in serv_addr;
  char buff[128];

  
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr,0,sizeof(struct sockaddr_in));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_addr.sin_port=htons(80);
  int flag=1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
  if(bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in))<0){
    perror("バインドに失敗");
    exit(EXIT_FAILURE);
  }
  if(listen(sockfd,5)<0){
    perror("接続待機中にエラー");
    exit(EXIT_FAILURE);
  }
  while(1){
  new_sockfd = accept(sockfd, NULL, NULL);
  
  FILE *istream;
  istream = fdopen(new_sockfd,"r++");
  setvbuf(istream, NULL, _IONBF, 0);
  while(1){
    if (fgets(buff, 1024, istream) == 0) break;
    printf("%s",buff);
    if(strcmp(buff,"\r\n") == 0) break;
  }
  fprintf(istream,"HTTP/1.1 200 OK\r\nContent-Type:text/html\r\n\r\nHello\r\n");
  
  
  sleep(1);
  fclose(istream);
  }
  close(sockfd);
}

