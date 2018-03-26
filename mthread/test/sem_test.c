#include<stdlib.h>
#include<mthread.h>
#include<unistd.h>
#include<stdio.h>
#include <sched.h>

mthread_sem_t mutex;

void* affichage (void* name) {
    int i, j;
    for(i = 0; i < 20; i++) {
    mthread_sem_wait(&mutex); /* prologue */
    for(j=0; j<5; j++) printf("%s ",(char*)name);
    //sched_yield(); /* pour etre sur d’avoir des problemes */
    for(j=0; j<5; j++) printf("%s ",(char*)name);
    printf("\n ");
    mthread_sem_post(&mutex); /* epilogue */
    }
return NULL;
}
int main (void) {
    mthread_t filsA, filsB;
    mthread_sem_init(&mutex, 1);

    if (mthread_create(&filsA, NULL, affichage, "AA")) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    if (mthread_create(&filsB, NULL, affichage, "BB")) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    void* res = NULL;
    if (mthread_join(filsA, &res))
        perror("mthread_join");
    if (mthread_join(filsB, &res))
        perror("mthread_join");
        
    printf("End of master thread\n") ;
    return (EXIT_SUCCESS);
}
