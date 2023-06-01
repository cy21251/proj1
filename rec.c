#include<stdio.h>
#include<signal.h>
#include<unistd.h>
void sig_handler(int signum){
	printf("zyusin");
}
int main(){
	
	while(1){
		signal(SIGUSR1,sig_handler);
		printf("z\n");
		sleep(1);
		}
}


