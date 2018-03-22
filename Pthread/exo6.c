#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

volatile char theChar = '\0';
volatile char print = 1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_read = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


void *read (void *name) {
	do {
		while (print == 1); /* wait my turn */
		pthread_mutex_lock(&lock);
		print = 1;
		theChar = getchar ();
		pthread_cond_signal(&cond);
		pthread_cond_wait(&cond_read,&lock);
		pthread_mutex_unlock(&lock);
	} while (theChar != 'F');
	return NULL;
}

void *write (void *name) {
	do {
		pthread_mutex_lock(&lock);
		print = 0;
		pthread_cond_wait(&cond,&lock);
		printf ("char = %c\n", theChar);
		pthread_cond_signal(&cond_read);
		pthread_mutex_unlock(&lock);
	} while (theChar != 'F');
	return NULL;
}


int main (void)
{
	pthread_t filsA, filsB;
	if (pthread_create (&filsA, NULL, write, (void*)"AA")) {
	perror ("pthread create");
	exit (1);
	}
	if (pthread_create (&filsB, NULL, read, (void*)"BB")) {
	perror ("pthread create");
	exit (1);
	}
	if (pthread_join (filsA, NULL))
	perror ("pthread join");
	if (pthread_join (filsB, NULL))
	perror ("pthread join");
	printf ("Fin du pere\n");
	return 0;
}