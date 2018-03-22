#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 10
#define SIZE 20

/* global thread exit control flag */
volatile int g_ExitFlag = 0;
sem_t lock;

struct arg_struct {
    int* begin;
    int size;
};


unsigned int max = 0;

void *  parallel_max(void *args){
   
   sem_wait(&lock);
   
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

   sem_init(&lock, 0, 1);

   pthread_t pid[NUM_THREADS];
   
   for(i = 0; i < NUM_THREADS; ++i){
      pthread_create(&pid[i], NULL, parallel_max, (void*)&arg[i]);
      pthread_detach(pid[i]);
   }

   /* run each thread four times and exit */
    for (i = 0; i < NUM_THREADS ; ++i)
    {

        /* release a thread to execute */
        sem_post(&lock);
        //sleep(1);
    }

   printf("Thread found max is %d\n",max );
   return 0;
}
