#include <errno.h>
#include "mthread_internal.h"



  /* Functions for mutex handling.  */

  /* Initialize MUTEX using attributes in *MUTEX_ATTR, or use the
     default values if later is NULL.  */
int
mthread_mutex_init (mthread_mutex_t * __mutex,
		    const mthread_mutexattr_t * __mutex_attr)
{
  //not_implemented ();
  if(__mutex==NULL)
    return -1;
  else{
  __mutex->nb_thread = 0 ; //intialize number of thread as zero
  __mutex->lock = 0;  // lock state should be unlocked (unsigned int type)
  __mutex->list = (struct mthread_list_s *)safe_malloc(sizeof(struct mthread_list_s));
   mthread_list_t INIT=MTHREAD_LIST_INIT;
  *(__mutex->list) = INIT;
  __mutex_attr = NULL; //Intialize as NULL  (No defination of this structure)

  return 0;
  }
  
}

  /* Destroy MUTEX.  */
int
mthread_mutex_destroy (mthread_mutex_t * __mutex)
{
  //not_implemented ();
  if(__mutex->nb_thread==0) /*mutex unlock*/
  {
    free(__mutex->list);  // delete it
  }
  else{
    //mthread_log("Dangerous to destroy a locked mutex\n");
    exit(1);
  }
  return 0;
}

  /* Try to lock MUTEX.  */
int
mthread_mutex_trylock (mthread_mutex_t * __mutex)
{
  //not_implemented ();
  if(__mutex == NULL || __mutex->list == NULL){
    return -EINVAL;
  }

  mthread_spinlock_lock(&(__mutex->lock));
  if(__mutex->nb_thread == 0)  //if unlock then lock it
  {
    __mutex->nb_thread = 1;
    mthread_spinlock_unlock(&(__mutex->lock));
    return 1;
  }
  else { 
    mthread_spinlock_unlock(&(__mutex->lock));
    return 0;
  }
}

  /* Wait until lock for MUTEX becomes available and lock it.  */
int
mthread_mutex_lock (mthread_mutex_t * __mutex)
{
  // 0 is opebn 1 is lock
  //not_implemented ();
  if(__mutex == NULL || __mutex->list == NULL){
    return -EINVAL;
  }
  mthread_t running;
  mthread_spinlock_lock(&(__mutex->lock));
  if(__mutex->nb_thread == 0)  //if unlock then lock it
  {
    __mutex->nb_thread = 1;
    mthread_spinlock_unlock(&(__mutex->lock));
  }
  else {  //if locked then add current thread to waiting list
    running = mthread_self();
    mthread_insert_first(running,__mutex->list);
    mthread_self()->status = BLOCKED;
    mthread_get_vp()->p = &__mutex->lock;
    //mthread_spinlock_unlock(&(__mutex->lock));
    mthread_yield();
  }
  return 0;
 
}

  /* Unlock MUTEX.  */
int
mthread_mutex_unlock (mthread_mutex_t * __mutex)
{
  //not_implemented ();
  if(__mutex == NULL || __mutex->list == NULL){
    return -EINVAL;
  }

  mthread_t thread;
  mthread_spinlock_lock(&(__mutex->lock));

  if(__mutex->list->first != NULL){  //check list if anyone is waiting
    thread = mthread_remove_first(__mutex->list);
    thread->status = RUNNING;
    mthread_virtual_processor_t *vp;
    vp = mthread_get_vp();
    mthread_insert_last(thread,&vp->ready_list); //Insert to ready list ready to go
  }else {  //Unlock mutex
      __mutex->nb_thread = 0; 
  }
  mthread_spinlock_unlock(&(__mutex->lock));
  return 0;
        
}