#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<error.h>
#include<pthread.h>
#include<signal.h>

void sys_err(const char* msg){
	perror(msg);
	exit(1);
}

void* tfn(void* arg){
	printf("thread: pid=%d,tid=%lu\n",getpid(),pthread_self());
	return NULL;
}

int main(int argc,char *argv[]){
	pthread_t tid;
	
	int ret=pthread_create(&tid,NULL,tfn,NULL);
	if(ret!=0){
		perror("pthread_create erro");
	}

	printf("main:pid=%d,tid=%lu\n",getpid(),pthread_self());
	//sleep(1);
	return 0;
}
