#include "mthread_internal.h"

  /* Functions for handling semaphore.  */

int
mthread_sem_init (mthread_sem_t * sem, unsigned int value)
{
  //not_implemented ();
  if(sem == NULL)
    return -1;
  else{
    sem->value = value ; //intialize number of thread as zero
    sem->lock = 0;  // lock state should be unlocked (unsigned int type)
    mthread_mutex_init(&(sem->m_lock),NULL);
    sem->leftToUnblock = 0;
  return 0;
  }
  return 0;
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
    if (sem->leftToUnblock <= 0){ //not availble
      //mthread_cond_wait(sem->cond,sem->mutex);
      mthread_mutex_lock(&(sem->m_lock));
    } else { //availble let it go
      sem->leftToUnblock--;
    }
    mthread_spinlock_unlock(&(sem->lock));
  }
  return 0;
}
  
  /*
  mutex_lock();
  if s-val <= 0
    cond_wait()
  value--;
  mutex_unlock();
  */


/* V(sem), signal(sem) */
int
mthread_sem_post (mthread_sem_t * sem)
{
  //not_implemented ();
  if(sem == NULL)
    return -1;
  else{
    mthread_spinlock_lock(&(sem->lock));
    
    if(sem->leftToUnblock < sem->value ){
      sem->leftToUnblock++;
      mthread_mutex_unlock(&(sem->m_lock));
    }
    mthread_spinlock_unlock(&(sem->lock));
  }
   return 0;
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
  not_implemented ();
  return 0;
}

/*
how to write a rwlocks with semaphore or mutex
mutex_lock()
if (w->cond >=1)
  w->value++
  


*/