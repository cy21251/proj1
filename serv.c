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
  serv_addr.sin_port=htons(50000);
  if(bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in))<0){
    perror("バインドに失敗");
    exit(EXIT_FAILURE);
  }
  if(listen(sockfd,5)<0){
    perror("接続待機中にエラー");
    exit(EXIT_FAILURE);
  }
  new_sockfd = accept(sockfd, NULL, NULL);
  if(read(new_sockfd, buff, 128)<0){
    perror("接続失敗");
    exit(EXIT_FAILURE);
  }
  printf("%s\n",buff);
  sleep(1);
  close(new_sockfd);
  close(sockfd);
}
