#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

/* 共享变量 */
int partA_count = 0;       // 工作台中零件A的数量
int partB_count = 0;       // 工作台中零件B的数量
int available_space = 12;  // 工作台中的空位数量

/* 互斥锁 */
pthread_mutex_t mutex_partA;     // 对partA_count的互斥锁
pthread_mutex_t mutex_partB;     // 对partB_count的互斥锁
pthread_mutex_t mutex_space;     // 对available_space的互斥锁，同时控制对工作台的访问

/* 信号量 */
sem_t sem_workerA;   // 用于挂起和唤醒workerA线程
sem_t sem_workerB;   // 用于挂起和唤醒workerB线程
sem_t sem_workerC;   // 用于挂起和唤醒workerC线程

/* workerA生产两个零件A并放入工作台 */
void *producerA(void* arg)
{
    while (1)
    {
        sleep(1);
        pthread_mutex_lock(&mutex_partA);
        pthread_mutex_lock(&mutex_space);
        if (available_space >= 2 && partA_count <= 7) // 工作台有空位且能保证放下3个零件B
        {
            // 向工作台放入两个零件A
            available_space -= 2;
            partA_count += 2;
            printf("-----------producerA-----------\n");
            printf("partA_count: %d, available_space: %d\n", partA_count, available_space);
            if (partA_count >= 4 && partB_count >= 3) // 如果workerC被挂起且工作台中有足够的零件A和零件B
            {
                sem_post(&sem_workerC); // 唤醒workerC线程
            }
            pthread_mutex_unlock(&mutex_space);
            pthread_mutex_unlock(&mutex_partA);
        }
        else
        {
            pthread_mutex_unlock(&mutex_space);
            pthread_mutex_unlock(&mutex_partA);
            printf("producerA is suspended\n");
            sem_wait(&sem_workerA);
            printf("producerA is resumed\n");
        }
    }
}

/* workerB生产一个零件B并放入工作台 */
void *producerB(void* arg)
{
    while (1)
    {
        sleep(1);
        pthread_mutex_lock(&mutex_partB);
        pthread_mutex_lock(&mutex_space);
        if (available_space >= 1 && partB_count <= 7) // 工作台有空位并能保证放下4个零件A
        {
            // 向工作台放入一个零件B
            available_space -= 1;
            partB_count += 1;
            printf("-----------producerB-----------\n");
            printf("partB_count: %d, available_space: %d\n", partB_count, available_space);
            if (partA_count >= 4 && partB_count >= 3) // 如果workerC被挂起且工作台中有足够的零件A和零件B
            {
                sem_post(&sem_workerC); // 唤醒workerC线程
            }
            pthread_mutex_unlock(&mutex_space);
            pthread_mutex_unlock(&mutex_partB);
        }
        else
        {
            pthread_mutex_unlock(&mutex_space);
            pthread_mutex_unlock(&mutex_partB);
            // 挂起workerB线程
            printf("producerB is suspended\n");
            sem_wait(&sem_workerB);
            printf("producerB is resumed\n");
        }
    }
}

/* workerC从工作台中取出4个零件A和3个零件B */
void *assembler(void* arg)
{
    while (1)
    {
        sleep(2);
        pthread_mutex_lock(&mutex_partA);
        pthread_mutex_lock(&mutex_partB);
        if (partA_count >= 4 && partB_count >= 3) // 工作台中有足够的零件A和零件B
        {
            pthread_mutex_lock(&mutex_space);
            // 从工作台中取出4个零件A和3个零件B
            partA_count -= 4;
            partB_count -= 3;
            available_space += 7;
            printf("-----------assemble-----------\n");
            printf("partA_count: %d, partB_count: %d, available_space: %d\n", partA_count, partB_count, available_space);
            pthread_mutex_unlock(&mutex_space);
            pthread_mutex_unlock(&mutex_partB);
            pthread_mutex_unlock(&mutex_partA);

            sem_post(&sem_workerA); // 唤醒workerA线程
            sem_post(&sem_workerB); // 唤醒workerB线程

        }
        else
        {
            pthread_mutex_unlock(&mutex_partB);
            pthread_mutex_unlock(&mutex_partA);
            printf("assembler is suspended\n");
            sem_wait(&sem_workerC);
            printf("assembler is resumed\n");
        }
    }
}

int main()
{
    /* 线程的标识符 */
    pthread_t threadA, threadB, threadC;
    int ret = 0;

    /* 初始化互斥锁 */
    pthread_mutex_init(&mutex_partA, NULL);
    pthread_mutex_init(&mutex_partB, NULL);
    pthread_mutex_init(&mutex_space, NULL);

    /* 初始化信号量 */
    sem_init(&sem_workerA, 0, 0);
    sem_init(&sem_workerB, 0, 0);
    sem_init(&sem_workerC, 0, 0);

    /* 分别创建线程A、B、C */
    ret = pthread_create(&threadA, NULL, producerA, NULL);
    if (ret != 0)
    {
        perror("producerA_create");
    }
    ret = pthread_create(&threadB, NULL, producerB, NULL);
    if (ret != 0)
    {
        perror("producerB_create");
    }
    ret = pthread_create(&threadC, NULL, assembler, NULL);
    if (ret != 0)
    {
        perror("assembler_create");
    }
    sleep(3);

    /* 等待线程A、B、C的结束 */
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    return 0;
}
