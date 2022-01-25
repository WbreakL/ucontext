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

void print_set(sigset_t* set){
	int i;
	for(i=1;i<32;i++){
		if(sigismember(set,i))
			putchar('1');
		else
			putchar('0');
		
	}
	printf("\n");
}
int main(int argc,char *argv[]){
	sigset_t set,oldset,pedset;
	int ret=0;
	sigemptyset(&set);
	sigaddset(&set,SIGINT);
	while(1){
	ret=sigprocmask(SIG_BLOCK,&set,&oldset);
	if(ret==-1)
		sys_err("sigpromask error");
	ret = sigpending(&pedset);
	if(ret==-1)
		sys_err("sigpending error");
	print_set(&pedset);
	sleep(1);
	}
	return 0;
}
