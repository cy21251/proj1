#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *func(void*arg){
  int i;
  for(i=0;i<10;i++){
    printf("child\n");
    sleep(1);
  }
}

int main(void){
  int i;
  int val;

  pthread_t thread_id;
  pthread_create(&thread_id,NULL,func,(void *)val);
  for(i=0;i<10;i++){
    printf("parent\n");
    sleep(1);
  }

  pthread_join(thread_id,NULL);
}
