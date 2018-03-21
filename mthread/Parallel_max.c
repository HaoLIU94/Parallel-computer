#include "mthread.h"
#include <stdio.h>
#include <stdlib.h>


#define COUNT_PER_THREAD 10000
#define NUM_THREADS 10
#define SIZE 20

struct arg_struct {
    int* begin;
    int size;
};

mthread_mutex_t lock;

unsigned int max = 0;

void *parallel_max(void *args){
   
   mthread_mutex_lock(&lock);
   
   struct arg_struct *arg = (struct arg_struct *)(args);
   printf("%d\n", * arg -> begin);
   printf("%d\n", * (arg -> begin+1));
   printf("%d\n", arg -> size);
      
   int res = *arg->begin > *(arg->begin+1) ? *arg->begin: *(arg->begin+1);

   if (res > max)
      max = res;

   mthread_mutex_unlock(&lock);
   return (void *)max;
}

int main(int argc, char** argv)
{   
   mthread_mutex_init(&lock,NULL);  
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

   mthread_t pid[NUM_THREADS];
   
   for(i = 0; i < NUM_THREADS; ++i){
      mthread_create(&pid[i], NULL, parallel_max, (void*)&arg[i]);
   }

   for(i = 0; i < NUM_THREADS; ++i){
      void* res = NULL;
      mthread_join(pid[i],&res );
      fprintf(stderr, "Thread found %d\n",(int)res );
   }

   mthread_mutex_destroy(&lock);

   printf("Thread found max is %d\n",max );
   
   return 0;
}
