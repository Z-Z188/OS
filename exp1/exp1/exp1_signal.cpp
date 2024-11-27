#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void handle_sig(int sig){
    printf("Caught signal %d\n", sig);
    if(sig == SIGALRM){
        alarm(2);
    }
}

int main() 
{ 
    signal(SIGINT, handle_sig);     // ctrl + c
    signal(SIGUSR1, handle_sig);    // a
    signal(SIGQUIT, handle_sig);    /* ctrl + \ */
    signal(SIGALRM, handle_sig);    // alarm
    alarm(2);
    while (1){
        char a = getchar();
        if(a == 'a'){
            kill(getpid(), SIGUSR1);
        }
    }
    return 0; 
} 