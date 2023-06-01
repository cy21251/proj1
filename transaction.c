#include<stdio.h>
#include<time.h>
#include<pthread.h>
#include<stdlib.h>
#define LOOP 10000
#define N 10000
struct timespec ts;
struct timespec x;
int from[N];
int to[N];
int amount[N];
int account[N];
pthread_mutex_t mutex;

void *func(void *arg){
  int i;
  int num =(int)arg;
  int now;

  for(i=0; i<1000; i++) {
    now=i+num*N/10;
    for(i=0;i<100;i++){}
    
    account[from[now]] = account[from[now]] - amount[now];
    account[to[now]] = account[to[now]] + amount[now];
  }
  
  
}

void busy(){
  int i;
  for(i=0;i<LOOP;i++){};
}

int sumAmount(){
  int sum =0;
  int i;
  for(i=0;i<N;i++){
    sum=sum+account[i];
  }
  return sum;
}

int main(void){
  char fname[]="trans.csv";
  int i;
  int val;
  char t;
  
  FILE *istream;
  istream=fopen(fname,"r");
  if(istream==NULL){
    printf("fopen error");
    exit(1);
  }
  for(i=0;i<N;i++){
    val=fscanf(istream,"%c,%d,%d,%d\n",&t,&from[i],&to[i],&amount[i]);
    if(val==EOF){
      perror("fscanf error");
      exit(1);
    }
  }
  
  val=fclose(istream);
  for(i=0;i<100;i++){
    account[i]=10000;
  }
  int before=sumAmount();
  printf("実行前の全口座総額:%d\n",before);
  
  clock_gettime(CLOCK_REALTIME,&ts);
  long startt=(x.tv_sec*1000000000)+x.tv_nsec;

  pthread_t thread_0;
  pthread_t thread_1;
  pthread_t thread_2;
  pthread_t thread_3;
  pthread_t thread_4;
  pthread_t thread_5;
  pthread_t thread_6;
  pthread_t thread_7;
  pthread_t thread_8;
  pthread_t thread_9;
  pthread_create(&thread_0,NULL,func,(void *)0);
  pthread_create(&thread_1,NULL,func,(void *)1);
  pthread_create(&thread_2,NULL,func,(void *)2);
  pthread_create(&thread_3,NULL,func,(void *)2);
  pthread_create(&thread_4,NULL,func,(void *)2);
  pthread_create(&thread_5,NULL,func,(void *)2);
  pthread_create(&thread_6,NULL,func,(void *)2);
  pthread_create(&thread_7,NULL,func,(void *)2);
  pthread_create(&thread_8,NULL,func,(void *)2);
  pthread_create(&thread_9,NULL,func,(void *)2);
  pthread_join(thread_0,NULL);
  pthread_join(thread_1,NULL);
  pthread_join(thread_2,NULL);
  pthread_join(thread_3,NULL);
  pthread_join(thread_4,NULL);
  pthread_join(thread_5,NULL);
  pthread_join(thread_6,NULL);
  pthread_join(thread_7,NULL);
  pthread_join(thread_8,NULL);
  pthread_join(thread_9,NULL);
  int after=sumAmount();
  printf("実行後の全口座総額:%d\n",after);
  printf("差:d\n",after-before);
  clock_gettime(CLOCK_REALTIME,&ts);
  long s=(ts.tv_sec*1000000000)+ts.tv_nsec;
  printf("%lf\n秒",s-startt);

  return 0;
}

