#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void pthread_cleanup(void* args){
	printf("Called clean-up handler with arg: %s\n", (char*)args);
}
 
void* RunfatherCancel(void* args)
{
	pthread_cleanup_push(pthread_cleanup, "hello_fathercancle");

    while(1){
        pthread_testcancel();
    }

    pthread_cleanup_pop(0);    
}
 
void* RunchildCancel(void* args)
{
	pthread_cleanup_push(pthread_cleanup, "hello_chilecancel");

    pthread_exit(NULL);

    pthread_cleanup_pop(0);    
}

void* RunPop(void* args){
    pthread_cleanup_push(pthread_cleanup, "hello_pop");

    pthread_cleanup_pop(1);    
}
int main(){
	pthread_t fatherCancel, childCancel, pop1;
	pthread_create(&fatherCancel, NULL, RunfatherCancel, NULL);
    pthread_create(&childCancel, NULL, RunchildCancel, NULL);
    pthread_create(&pop1, NULL, RunPop, NULL);
    
	sleep(1);
    pthread_cancel(fatherCancel);
	pthread_join(fatherCancel, NULL);
	pthread_join(childCancel, NULL);
    pthread_join(pop1, NULL);
    return 0;
}