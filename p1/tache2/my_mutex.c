#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_mutex.h"

my_mutex_t mutx;
//int nAccess;

int my_mutex_init(my_mutex_t* mutex) {
    mutex->value = 0;
}

int my_mutex_lock_ts(my_mutex_t* mutex) {
    int* value = &(mutex->value);
    __asm__(
        "ENTER1:\n\t"
        "movl $1, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        "testl %%eax, %%eax\n\t"
        "jnz ENTER1\n\t"
        :"+m" (*value)
        :
        :"%eax"
    );
}

int my_mutex_lock_tts(my_mutex_t* mutex) {
    int* value = &(mutex->value);
    __asm__(
        "ENTER2:\n\t"
        "movl $1, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        "testl %%eax, %%eax\n\t"
        "jz FIN\n\t"
        "LOOP:\n\t"
        "movl %0, %%eax\n\t" //added
        "testl %%eax, %%eax\n\t" //changed
        "jnz LOOP\n\t"
        "jmp ENTER2\n\t"
        "FIN:\n\t"
        :"+m" (*value)
        :
        :"%eax"

    );
    
}

int my_mutex_unlock(my_mutex_t* mutex) {
    int *val = &(mutex->value);
    __asm__(
        "movl $0, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        :"+m" (*val)
        :
        :"%eax"
    );
}

int my_sem_init(my_sem_t* sem, int val) {
    sem->value = val;
    my_mutex_init(&(sem->modifying));
}

int my_sem_wait(my_sem_t* sem) {
    while (1) {
        my_mutex_lock_tts(&(sem->modifying));
        if(sem->value > 0) {
            sem->value--;
            my_mutex_unlock(&(sem->modifying));
            break;
        }
        my_mutex_unlock(&(sem->modifying));
    }
}

int my_sem_post(my_sem_t *sem) {
    my_mutex_lock_ts(&(sem->modifying));
    sem->value++;
    my_mutex_unlock(&(sem->modifying));
}

void* func (void* arg) {
    int nAccess = *((int*) arg);
    for (int i = 0; i < nAccess; i++) {
        my_mutex_lock_ts(&mutx);
        for (int i = 0; i < 10000; i++) {}
        my_mutex_unlock(&mutx);
    }
    
}
/*

int main(int argc, char *argv[]) {
    int nThreads = atoi(argv[1]);
    int nAccess = 6400 / nThreads;

    my_mutex_init(&mutx);

    void* res;
    pthread_t threads[nThreads];
    for (int i = 0; i < nThreads; i++) {
        pthread_create(threads + i, NULL, &func, &nAccess);
    }
    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], &res);
    }
    
}
*/