#include <pthread.h>
#include <stdio.h>


#define COUNT_PER_THREAD 1000000000
#define NUM_THREADS 4
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

unsigned long count = 0;

void *  parallel_count(void *args){
   
   //pthread_mutex_lock(&lock);
   int i;
   
   for(i=0; i < COUNT_PER_THREAD; ++i){
      ++count;
   }
   //pthread_mutex_unlock(&lock);
   return NULL;
}



int main(int argc, char** argv)
{
   unsigned long i;
   pthread_t pid[NUM_THREADS];
   
   
   for(i = 0; i < NUM_THREADS; ++i){
      pthread_create(&pid[i], NULL, parallel_count, NULL);
   }
   
   for(i = 0; i < NUM_THREADS; ++i){
      void* res;
      pthread_join(pid[i], NULL);
   }

   /*
   for(i=0; i < NUM_THREADS*COUNT_PER_THREAD; ++i){
      ++count;
   }
   */

   printf("Final count %ld\n", count);
   
   return 0;
}
