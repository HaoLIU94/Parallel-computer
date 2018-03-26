#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct arg_struct {
    int id;
    int n;
};

void *sum_thread(void *args){
   
   pthread_mutex_lock(&lock);
   struct arg_struct *arg = (struct arg_struct *)(args);
   int i;
   int res = 0;
   for(i = 0 ; i <  arg->n ; i++)
   {
       res += i;
   }
   pthread_mutex_unlock(&lock);
   return (void*)res;
}

int main(int argc, char** argv)
{
   int i;
   int N = atoi(argv[1]);

   pthread_t pid[NUM_THREADS];

   struct arg_struct arg[NUM_THREADS];

   for (int i = 0; i < NUM_THREADS; ++i)
   {
      arg[i].id = i;
      arg[i].n = N;
   }
   
   
   for(i = 0; i < NUM_THREADS; ++i){
      pthread_create(&pid[i], NULL, sum_thread, (void*)&(arg[i]));
   }

   int sum_loc[NUM_THREADS] = {0};

   for(i = 0; i < NUM_THREADS; ++i){
      void* res;
      pthread_join(pid[i],&res );
      sum_loc[i]=(int*)res;
   }
   int sum=0;
   for(i = 0; i < NUM_THREADS; i++){
      //printf("%p ,%d",&sum_loc[i],i);
      sum += sum_loc[i];
   }
   printf("Sum = %d \n", sum);
   
   return 0;
}
