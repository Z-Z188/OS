#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void* asynStrlen(void* str){
    int64_t len = strlen((char*)str);
    pthread_exit((void*)len);
}
void* negative(void* a){
    int64_t t = *(int*)a;
    t = -t;
    pthread_exit((void*)t);
}

typedef struct addPara
{
    int a, b;
} addPara;

void *add(void* t){
    addPara* m = (addPara*)t;
    int64_t ans = m->a + m->b;
    pthread_exit((void*)ans);
}

int main(){
    char h[] = "hello";
    int a = 10, b = 20;
    void *h_len, *a_neg, *sum;
    pthread_t threadStrlen, threadNeg, threadAdd;

    addPara t;
    t.a = a;
    t.b = b;
    pthread_create(&threadStrlen, NULL, asynStrlen, h);
    pthread_create(&threadNeg, NULL, negative, &a);
    pthread_create(&threadAdd, NULL, add, &t);

    pthread_join(threadStrlen, &h_len);
    pthread_join(threadNeg, &a_neg);
    pthread_join(threadAdd, &sum);

    printf("Length of h is %ld\n", (int64_t)h_len);
    printf("negative of a is %d\n", (int)(int64_t)a_neg);
    printf("sum of a and b is %d\n", (int)(int64_t)sum);
    
    return 0;
}