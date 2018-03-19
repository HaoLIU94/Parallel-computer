#include "mthread_internal.h"

  /* Functions for handling conditional variables.  */

  /* Initialize condition variable COND using attributes ATTR, or use
     the default values if later is NULL.  */
int
mthread_cond_init (mthread_cond_t * __cond,
		   const mthread_condattr_t * __cond_attr)
{
  not_implemented ();
  return 0;
}

  /* Destroy condition variable COND.  */
int
mthread_cond_destroy (mthread_cond_t * __cond)
{
  not_implemented ();
  return 0;
}

  /* Wake up one thread waiting for condition variable COND.  */
int
mthread_cond_signal (mthread_cond_t * __cond)
{
  //not_implemented ();
  if(__cond == NULL){
    return -1;
  }
  mthread_spinlock_lock(&(__cond->lock));
  if(__cond->cond == 1){
    mthread_t thread;
    if(__cond->list->first != NULL){  //check list
      thread = mthread_remove_first(__cond->list);
      mthread_virtual_processor_t *vp;
      vp = mthread_get_vp();
      mthread_insert_last(thread,&vp->ready_list);
    }
  }
  mthread_spinlock_unlock(&(__cond->lock));
  return 0;
}

  /* Wake up all threads waiting for condition variables COND.  */
int
mthread_cond_broadcast (mthread_cond_t * __cond)
{
  //not_implemented ();
  if(__cond == NULL){
    return -1;
  }
  mthread_spinlock_lock(&(__cond->lock));
  if(__cond->cond == 1){
   
    mthread_t thread;
    while(__cond->list->first != NULL){  //check list
      thread = mthread_remove_first(__cond->list);
      mthread_virtual_processor_t *vp;
      vp = mthread_get_vp();
      mthread_insert_last(thread,&vp->ready_list);
    }
  }
  mthread_spinlock_unlock(&(__cond->lock));
  return 0;
}

  /* Wait for condition variable COND to be signaled or broadcast.
     MUTEX is assumed to be locked before.  */
int
mthread_cond_wait (mthread_cond_t * __cond, mthread_mutex_t * __mutex)
{
  //not_implemented ();
  if(__cond == NULL){
    return -1;
  }
  mthread_spinlock_lock(&(__cond->lock));
  mthread_mutex_lock(__mutex);
  if(__cond->cond ==1){
    mthread_t running;
    running = mthread_self();
    mthread_insert_first(running,__cond->list);
  }
  mthread_spinlock_lock(&(__cond->lock));
  return 0;
}
