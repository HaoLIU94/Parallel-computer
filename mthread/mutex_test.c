#include "mthread.h"
#include <pthread.h>
#include <stdio.h>

mthread_mutex_t lock;

#define COUNT_PER_THREAD 10000
#define NUM_THREADS 10

unsigned long count = 0;

void *  parallel_count(void *args){

  mthread_mutex_lock(&lock);
   int i;
   for(i=0; i < COUNT_PER_THREAD; ++i){
      ++count;
   }
  mthread_mutex_unlock(&lock);
  printf("count %ld \n", count);
  return NULL;
}

int main(int argc, char** argv)
{
   int i;
   mthread_t pid[NUM_THREADS];
   mthread_mutex_init(&lock,NULL);   

   for(i = 0; i < NUM_THREADS; ++i){
      mthread_create(&pid[i], NULL, parallel_count, NULL);
   }
   for(i = 0; i < NUM_THREADS; ++i){
      void* res;
      mthread_join(pid[i], &res);
   }
   mthread_mutex_destroy(&lock);
   printf("Valeur finale de count %ld \n", count);
   
   return 0;
}
