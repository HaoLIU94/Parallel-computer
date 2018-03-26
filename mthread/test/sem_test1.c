#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "mthread.h"

mthread_sem_t semP, semC;
int stock_count = 0;
const int stock_max_limit=5;

void *producer(void *arg) {
    int i;
    for (i = 0; i < 10; i++) {

        while(stock_max_limit == stock_count){
            printf("stock overflow, production on wait..\n");
            mthread_sem_wait(&semC);
            printf("production operation continues..\n");
        }

        sleep(1);   //production decided here
        stock_count++;
        printf("P::stock-count : %d\n",stock_count);
        mthread_sem_post(&semP);
        printf("P::post signal..\n");
    }
    return NULL;
}

void *consumer(void *arg) {
    int i;
    for (i = 0; i < 10; i++) {

        while(0 == stock_count){
            printf("stock empty, consumer on wait..\n");
            mthread_sem_wait(&semP);
            printf("consumer operation continues..\n");
        }

        sleep(2);   //consumer rate decided here
        stock_count--;
        printf("C::stock-count : %d\n", stock_count);
        mthread_sem_post(&semC);
        printf("C::post signal..\n");
    }
    return NULL;
}

int main(void) {

    mthread_t tid0,tid1;
    mthread_sem_init(&semP, 1);
    mthread_sem_init(&semC, 1);

        mthread_create(&tid0, NULL, consumer, NULL);
        mthread_create(&tid1, NULL, producer, NULL);
        void* res = NULL;
        mthread_join(tid0, &res);
        mthread_join(tid1, &res);

    mthread_sem_destroy(&semC);
    mthread_sem_destroy(&semP);

    return 0;
}
