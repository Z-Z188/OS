#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int shared_data = 0;
pthread_mutex_t lock;
bool thread_3finish = false;
void *thread_func1(void *arg) {
    sleep(rand() % 10);
    pthread_mutex_lock(&lock);
    shared_data += 10;
    printf("Thread %ld: Shared data = %d\n",pthread_self(), shared_data);
    pthread_mutex_unlock(&lock);
    return NULL;
}

void *thread_func2(void *arg) {
    sleep(rand() % 10);
    pthread_mutex_lock(&lock);
    shared_data += 20;
    printf("Thread %ld: Shared data = %d\n",pthread_self(), shared_data);
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

void *thread_func3(void *arg) {
    sleep(rand() % 10);
    pthread_mutex_lock(&lock);
    shared_data += 30;
    printf("Thread %ld: Shared data = %d\n",pthread_self(), shared_data);
    pthread_mutex_unlock(&lock);
    thread_3finish = true;
    while(true);
}

int main() {
    pthread_t threads[3];
    srand(time(NULL));

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
        return 1;
    }
    
    pthread_create(&threads[0], NULL, thread_func1, NULL);
    printf("father create thread with ID : %ld\n", (u_int64_t)threads[0]);
    pthread_create(&threads[1], NULL, thread_func2, NULL);
    printf("father create thread with ID : %ld\n", (u_int64_t)threads[1]);
    pthread_create(&threads[2], NULL, thread_func3, NULL);
    printf("father create thread with ID : %ld\n", (u_int64_t)threads[2]);

    printf("Create 3 threads\n");

    while(!thread_3finish);
    pthread_cancel(threads[2]);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    printf("Parent thread: Final shared data = %d\n", shared_data);

    pthread_mutex_destroy(&lock);
    return 0;
}
