#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>


int main(){
	int dt;
	scanf("%d",&dt);
	kill(dt,SIGUSR1);
	printf("soushin");
}
