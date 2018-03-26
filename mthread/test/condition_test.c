#include <mthread.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>


/* Compile like this:
 gcc --std=c99 -lpthread cond.c -o cond
*/

const size_t NUMTHREADS = 10;

/* a global count of the number of threads finished working. It will
   be protected by mutex and changes to it will be signalled to the
   main thread via cond */

int done = 0;
mthread_mutex_t mutex;
mthread_cond_t cond;

/* Note: error checking on pthread_X calls ommitted for clarity - you
   should always check the return values in real code. */

/* Note: passing the thread id via a void pointer is cheap and easy,
 * but the code assumes pointers and long ints are the same size
 * (probably 64bits), which is a little hacky. */

void barrier(void)
{
  static int barriercount = 0;

  mthread_mutex_lock(&mutex);
  barriercount++;
  printf("Thread %d in barrier, max is %zu threads in barrier\n",barriercount,NUMTHREADS );

  if (barriercount < NUMTHREADS){
    mthread_cond_wait(&cond,&mutex);
  }
  else{
    mthread_cond_broadcast(&cond);
    barriercount=0;
  }
  mthread_mutex_unlock(&mutex);
}


void* ThreadEntry( void* id )
{
  const int myid = (long)id; // force the pointer to be a 64bit integer
  
  const int workloops = 3;
  for( int i=0; i<workloops; i++ )
    {
      printf( "[thread %d] working (%d/%d)\n", myid, i, workloops );
      sleep(1); // simulate doing some costly work
    }
  barrier();

  return NULL;
}

int main( int argc, char** argv )
{
  puts( "[thread main] starting" );

  mthread_t threads[NUMTHREADS];

  mthread_mutex_init(&mutex,NULL);
  mthread_cond_init(&cond,NULL);

  for( int t=0; t<NUMTHREADS-1; t++ )
    mthread_create( &threads[t], NULL, ThreadEntry, (void*)(long)t );
  
  barrier();
  
  void* res = NULL;
  for( int t=0; t<NUMTHREADS-1; t++ )
    mthread_join(threads[t], &res);

  return 0;
}