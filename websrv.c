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
  int m,n;
  char buf[128];
  int i=0;

  
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
      printf("%s\n",buff);
      if(strncmp(buff,"GET",3)==0){
	m=1;
      }
      if(strncmp(buff,"POST",4)==0){
	m=2;
      }
      if(strncmp(buff,"Content-Length: ",16)==0){
	sscanf(buff,"Content-Length: %d",&n);
      }
      if(strcmp(buff,"\r\n") == 0) break;
    }
    if(m==1){
      fprintf(istream,"HTTP/1.1 200 OK\r\nContent-Type:text/html\r\n\r\n<form action='post.html' method='post'>\r\naddress<input type='text' name='addr'>r\n<input type='submit'value='send'>\r\n</form>\r\n");
    }
    else{
      if(fgets(buf,n+1,istream)==NULL){
	perror("fgets");
      }
      printf("%s\n",buf);
      fprintf(istream,"HTTP/1.1 200 OK\r\nContent-Type:text/html\r\n\r\nOK\r\n");
    }
    sleep(1);
    fclose(istream);
  }
  close(sockfd);
}
