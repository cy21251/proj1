//プログラム 1A
#include<stdio.h>
#include<time.h>
#include<pthread.h>
#define COUNT 10
#define LOOP 10000
struct timespec ts[5][COUNT];
struct timespec x;
pthread_mutex_t mutex;

void *func(void *arg){//処理A
  int i;
  int thn=(int)arg;

  pthread_mutex_lock(&mutex);//murexの獲得
  for(i=0;i<COUNT;i++){//処理Aをcount回繰り返す
    busy();
    clock_gettime(CLOCK_REALTIME,&ts[thn][i]);//時間測定
  }
  pthread_mutex_unlock(&mutex);//murexの開放
}

void busy(){
  int i;
  for(i=0;i<LOOP;i++){};
}

int main(void){
  int i;
  int j;
  pthread_t t[5];
  int thn;

  clock_gettime(CLOCK_REALTIME,&x);
  long startt=(x.tv_sec*1000000000)+x.tv_nsec;//現在時刻の獲得

  for(i=0;i<10;i++){//処理Aと処理Bを10回繰り返す
    pthread_create(&t[0],NULL,func,(void *)0);
    pthread_create(&t[1],NULL,func,(void *)1);
    pthread_create(&t[2],NULL,func,(void *)2);
    pthread_create(&t[3],NULL,func,(void *)3);
    pthread_join(t[0],NULL);
    pthread_join(t[1],NULL);
    pthread_join(t[2],NULL);
    pthread_join(t[3],NULL);
     //結果の記録
    for(thn=0;thn<4;thn++){
      for(j=0;j<COUNT;j++){
	long t=(ts[thn][j].tv_sec*1000000000)+ts[thn][j].tv_nsec;
	printf("%ld\t%d\n",t-startt,thn);
      }
    }
    //処理B
    busy();
  }
 
}
