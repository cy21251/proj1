#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
int new_sockfd;

void *func(void*arg){
  
  char buff[128];
    
  FILE *istream;
  istream = fdopen(new_sockfd,"r++");
  setvbuf(istream, NULL, _IONBF, 0);
  printf("a\n");
  while(1){
    if (fgets(buff, 1024, istream) == 0) break;
    //printf("%s\n",buff);
    if(strcmp(buff,"\r\n") == 0) break;
  }
  sleep(10);
  fprintf(istream,"HTTP/1.1 200 OK\r\nContent-Type:text/html\r\n\r\nOK\r\n");
  printf("b\n");
  sleep(1);
  fclose(istream);
}

int main(void){
  int i;
  int val;
  pthread_t t;
  
  struct sockaddr_in serv_addr;
  int sockfd;
  
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
  
  while(1){
    if(listen(sockfd,5)<0){
    perror("接続待機中にエラー");
    exit(EXIT_FAILURE);
    }
    new_sockfd = accept(sockfd, NULL, NULL);
    pthread_create(&t,NULL,func,(void *)val);
    pthread_detach(t);
    printf("c\n");
  }
  close(sockfd);
}
