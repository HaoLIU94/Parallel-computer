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

/*
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mthread.h"

#define NUM_THREADS 10
#define SIZE 20

mthread_sem_t lock;

struct arg_struct {
    int* begin;
    int size;
};


unsigned int max = 0;

void *  parallel_max(void *args){
   
   mthread_sem_wait(&lock);
   
   struct arg_struct *arg = (struct arg_struct *)(args);
   printf("%d\n", * arg -> begin);
   printf("%d\n", * (arg -> begin+1));
   printf("%d\n", arg -> size);
      
   int res = *arg->begin > *(arg->begin+1) ? *arg->begin: *(arg->begin+1);

   if (res > max)
      max = res;
   return NULL;
}

int main(int argc, char** argv)
{
   int i;

   int r[SIZE];

   for (int i = 0; i < SIZE; ++i)
   {
      r[i] = rand() % 1000;
   }

   struct arg_struct arg[SIZE];

   for (int i = 0; i < NUM_THREADS; ++i)
   {
      arg[i].begin = r+i*2;
      arg[i].size = 2;
   }

   mthread_sem_init(&lock, 1);

   mthread_t pid[NUM_THREADS];
   
   for(i = 0; i < NUM_THREADS; ++i){
      mthread_create(&pid[i], NULL, parallel_max, (void*)&arg[i]);
      //mthread_detach(pid[i]);
   }

   // run each thread four times and exit 
    for (i = 0; i < NUM_THREADS ; ++i)
    {
        // release a thread to execute 
        mthread_sem_post(&lock);
    }

   printf("Thread found max is %d\n",max );
   return 0;
}
*/