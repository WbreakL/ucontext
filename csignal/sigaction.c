#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<error.h>
#include<pthread.h>
#include<signal.h>

void sys_err(const char* msg) {
	perror(msg);
	exit(1);
}


void handler(int i) {
	printf("catch you!! %d\n", i);
}
int main(int argc, char* argv[]) {
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&(act.sa_mask));
	act.sa_flags = 0;

	int ret = sigaction(SIGINT, &act, 0);
	if (ret == -1)
		sys_err("sigaction error");
	while (1);
	return 0;
}
