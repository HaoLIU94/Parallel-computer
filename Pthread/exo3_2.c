#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define COUNT_PER_THREAD 10000
#define NUM_THREADS 10
#define SIZE 20

struct arg_struct {
    int* begin;
    int size;
};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

unsigned int max = 0;

void *  parallel_max(void *args){
   
   pthread_mutex_lock(&lock);
   
   struct arg_struct *arg = (struct arg_struct *)(args);
   printf("%d\n", * arg -> begin);
   printf("%d\n", * (arg -> begin+1));
   printf("%d\n", arg -> size);
      
   int res = *arg->begin > *(arg->begin+1) ? *arg->begin: *(arg->begin+1);

   if (res > max)
      max = res;

   pthread_mutex_unlock(&lock);
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

   pthread_t pid[NUM_THREADS];
   
   for(i = 0; i < NUM_THREADS; ++i){
      pthread_create(&pid[i], NULL, parallel_max, (void*)&arg[i]);
   }

   for(i = 0; i < NUM_THREADS; ++i){
      pthread_join(pid[i],NULL );
   }

   printf("Thread found max is %d\n",max );
   
   return 0;
}
