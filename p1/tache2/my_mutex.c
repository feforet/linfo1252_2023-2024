#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_mutex.h"

my_mutex_t mutx;

static inline int my_test_and_set(int* verrou) {
    __asm__(
        "ENTER:\n\t"
        "movl $1, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        "testl %%eax, %%eax\n\t"
        "jnz ENTER\n\t"
        :"+m" (*verrou)
        :
        :"%eax"
    );
}

int my_mutex_init(my_mutex_t* mutex) {
    mutex->value = 0;
}

int my_mutex_lock_ts(my_mutex_t* mutex) {
    my_test_and_set(&(mutex->value));
}

int my_mutex_lock_tts(my_mutex_t* mutex) {
    while (my_test_and_set(&(mutex->value))) {
        while (mutex->value) {}
    }
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
    my_mutex_init(sem->mutex);
}

int my_sem_wait(my_sem_t* sem) {
    my_mutex_lock_ts(sem->mutex);
    while (sem->value <= 0) {
        my_mutex_unlock(sem->mutex);
        my_mutex_lock_ts(sem->mutex);
    }
    sem->value--;
    my_mutex_unlock(sem->mutex);
}

int my_sem_post(my_sem_t *sem) {
    my_mutex_lock_ts(sem->mutex);
    sem->value++;
    my_mutex_unlock(sem->mutex);
}

void* func (void* arg) {
    int nAccess = *((int*) arg);
    for (int i = 0; i < nAccess; i++) {
        my_mutex_lock_ts(&mutx);
        for (int i = 0; i < 10000; i++) {}
        
        my_mutex_unlock(&mutx);
    }
    
}

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