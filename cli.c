#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include <arpa/inet.h>

int main(){
  struct sockaddr_in serv_addr; 
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  char buff[128];
  if(sockfd<0){
    perror("ソケットの作成失敗");
    exit(EXIT_FAILURE);
  }
  memset(&serv_addr, 0, sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr=inet_addr("172.28.34.70");
  serv_addr.sin_port = htons(50000);
  if(connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in))<0){
    perror("接続失敗");
    exit(EXIT_FAILURE);
  }
  printf("文字を入力\n");
  scanf("%c",buff);
  if(write(sockfd, buff, 128)<0){
    perror("メッセージの送信に失敗");
    exit(EXIT_FAILURE);
  }
  close(sockfd);
  
  return 0;
}
