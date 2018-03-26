#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <mthread.h>

/* compile with gcc -pthread lockwait.c */

mthread_cond_t cv;
mthread_mutex_t lock;

void *thread(void *v) {
	printf("Locking and waiting. Type unlock to unlock me.\n");
	mthread_mutex_lock(&lock);
	mthread_cond_wait(&cv, &lock);
	printf("I've been unlocked.\n");
	mthread_mutex_unlock(&lock);
	return NULL;
}

int main() {

	char cmd[1024];
	mthread_t *t;
	mthread_cond_init (&cv, NULL);
	mthread_mutex_init (&lock, NULL);


	printf("Type lock to run a thread that locks and waits.\n");
	printf("Type unlock to unlock the same thread.\n");
	while(fscanf(stdin, "%s", cmd) != EOF) {
		if(strcmp(cmd, "lock") == 0) { 
			t = (mthread_t *) malloc(sizeof(mthread_t));
			mthread_create(t, NULL, thread, NULL);		
		} else if(strcmp(cmd, "unlock") == 0) {
			mthread_cond_signal(&cv);
		} else if(strcmp(cmd, "end") == 0) {
			break;
		}

	}	
	return 0;
}