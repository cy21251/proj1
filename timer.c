#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/time.h>

void sig_handler(int signum){
	printf("hello\n");
}
int main(){
  signal(SIGALRM,sig_handler);
  struct itimerval timval;
  timval.it_interval.tv_sec=0;
  timval.it_interval.tv_usec=0;
  timval.it_value.tv_sec=1;
  timval.it_value.tv_usec=0;
  setitimer(ITIMER_REAL, &timval, NULL);
  while(1){
    printf("z\n");
    sleep(1);
  }
}


