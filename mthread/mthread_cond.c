#include "mthread_internal.h"

  /* Functions for handling conditional variables.  */

  /* Initialize condition variable COND using attributes ATTR, or use
     the default values if later is NULL.  */
int
mthread_cond_init (mthread_cond_t * __cond,
		   const mthread_condattr_t * __cond_attr)
{
  //not_implemented ();
  if(__cond == NULL)
    return -1;
  else{
    __cond->lock = 0;  // lock state should be unlocked (unsigned int type)
    __cond->list = (struct mthread_list_s *)safe_malloc(sizeof(struct mthread_list_s));
    mthread_list_t INIT=MTHREAD_LIST_INIT;
    *(__cond->list) = INIT;
    return 0;
  }
}

  /* Destroy condition variable COND.  */
int
mthread_cond_destroy (mthread_cond_t * __cond)
{
  if(__cond == NULL)
  {
    return 0;
  }
  if(__cond->list != NULL) /*mutex unlock*/
  {
    free(__cond->list);  // delete it
  }
  else{
    //mthread_log("Dangerous to destroy a locked mutex\n");
    exit(-1);
  }
  return 0;
}

  /* Wake up one thread waiting for condition variable COND.  */
int
mthread_cond_signal (mthread_cond_t * __cond)
{
  //not_implemented ();
  //not_implemented ();
  if(__cond == NULL)
    return -1;
  else{
    mthread_spinlock_lock(&(__cond->lock));
    mthread_t thread;
    if(__cond->list->first != NULL){  //check waiting list, activate the first one
      thread = mthread_remove_first(__cond->list);
      thread->status = RUNNING;
      mthread_virtual_processor_t *vp;
      vp = mthread_get_vp();
      mthread_insert_last(thread,&vp->ready_list); //Insert to ready list ready to go
    }else{
      __cond->cond = 0;  //no one is left
    } 
    mthread_spinlock_unlock(&(__cond->lock));
    return 0;
  }
  return 0;
}

  /* Wake up all threads waiting for condition variables COND.  */
int
mthread_cond_broadcast (mthread_cond_t * __cond)
{
  //not_implemented ();
  if(__cond == NULL)
    return -1;
  else{
    mthread_spinlock_lock(&(__cond->lock));
    mthread_t thread;
    while(__cond->list->first != NULL){  //check waiting list, activate them all
      thread = mthread_remove_first(__cond->list);
      thread->status = RUNNING;
      mthread_virtual_processor_t *vp;
      vp = mthread_get_vp();
      mthread_insert_last(thread,&vp->ready_list); //Insert to ready list ready to go
    }
    mthread_spinlock_unlock(&(__cond->lock));
    return 0;
  }
}

  /* Wait for condition variable COND to be signaled or broadcast.
     MUTEX is assumed to be locked before.  */
int
mthread_cond_wait (mthread_cond_t * __cond, mthread_mutex_t * __mutex)
{
  //not_implemented ();
  if(__cond == NULL)
    return -1;
  else{
    mthread_spinlock_lock(&(__cond->lock));
    /*
    if( __mutex != NULL)
    {
       __cond->cond = __mutex->nb_thread; //if mutex is locked so our condition is set locked as well
    }
    */
    mthread_t running;
    if (__cond->cond == 0 ){  //Condition verified (unlock) let it go
      mthread_spinlock_unlock(&(__cond->lock)); 
    }else{  //if locked then add current thread to cond waiting list
      running = mthread_self();
      mthread_insert_first(running,__cond->list);
      mthread_self()->status = BLOCKED;
      mthread_get_vp()->p = &__cond->lock;
      mthread_yield();
    }
    return 0;
  } 
}
