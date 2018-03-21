#include "mthread_internal.h"

  /* Functions for handling semaphore.  */

int
mthread_sem_init (mthread_sem_t * sem, unsigned int value)
{
  //not_implemented ();
  if(sem == NULL)
    return -1;
  else{
    sem->init_value = value ; //intialize number of thread as zero
    sem->lock = 0;  // lock state should be unlocked (unsigned int type)
    sem->leftToUnblock = value;
    sem->list = (struct mthread_list_s *)safe_malloc(sizeof(struct mthread_list_s));
    mthread_list_t INIT=MTHREAD_LIST_INIT;
    *(sem->list) = INIT;
    return 0;
  }
}

/* P(sem), wait(sem) */
int
mthread_sem_wait (mthread_sem_t * sem)
{
  //not_implemented ();
  if(sem == NULL)
    return -1;
  else{
    mthread_spinlock_lock(&(sem->lock));
    //mthread_cond_wait(sem->cond,sem->mutex);
    mthread_t running;
    if (sem->leftToUnblock > 0){  //Greater than zero let it go
      sem->leftToUnblock--;       //Decrease the value
      mthread_spinlock_unlock(&(sem->lock)); 
    }
    else {  //if locked then add current thread to waiting list
      running = mthread_self();
      mthread_insert_first(running,sem->list);
      mthread_self()->status = BLOCKED;
      mthread_get_vp()->p = &sem->lock;
      mthread_yield();
    }
    return 0;
  } 
} 

/* V(sem), signal(sem) */
int
mthread_sem_post (mthread_sem_t * sem)
{
  //not_implemented ();
  if(sem == NULL)
    return -1;
  else{
    mthread_spinlock_lock(&(sem->lock));
    mthread_t thread;
    if(sem->list->first != NULL && sem->leftToUnblock < sem->init_value ){  //check list if anyone is waiting
      sem->leftToUnblock++;
      thread = mthread_remove_first(sem->list);
      thread->status = RUNNING;
      mthread_virtual_processor_t *vp;
      vp = mthread_get_vp();
      mthread_insert_last(thread,&vp->ready_list); //Insert to ready list ready to go
    }
    mthread_spinlock_unlock(&(sem->lock));
    return 0;
  }
}

  /*
  mutex_lock();
  value++;
  cond_signal();
  mutex_unlock();
  */

int
mthread_sem_getvalue (mthread_sem_t * sem, int *sval)
{
  not_implemented ();
  return 0;
}

int
mthread_sem_trywait (mthread_sem_t * sem)
{
  not_implemented ();
  return 0;
}

/* undo sem_init() */
int
mthread_sem_destroy (mthread_sem_t * sem)
{
  if(sem->init_value==sem->leftToUnblock) /*mutex unlock*/
  {
    free(sem->list);  // delete it
  }
  else{
    //mthread_log("Dangerous to destroy a locked mutex\n");
    exit(-1);
  }
  return 0;
}
