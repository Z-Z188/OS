#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#define FIBER_STACK 8192
int a;
void * stack;
int do_something(void *arg){
	a=10;
	sleep(1);
	printf("This is son, the pid is:%d, the a is: %d\n", getpid(), a);
	free(stack); 
	exit(1);
}

int main() {
	void * stack;
	a = 1;
	stack = malloc(FIBER_STACK);	// memory for chile process
	if(!stack) {
		printf("The stack failed\n");
		exit(0);
	}

	printf("creating son thread!!!\n");
	pid_t son_pid = clone(&do_something, (char *)stack + FIBER_STACK, CLONE_VM|CLONE_VFORK, 0); // create child process
	printf("This is father, my pid is: %d, the a is: %d, son_pid = %d\n", getpid(), a, son_pid);
	exit(1);
}
