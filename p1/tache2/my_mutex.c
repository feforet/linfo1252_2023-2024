#include <stdio.h>
#include <stdlib.h>
#include "my_mutex.h"

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

int pthread_mutex_init(my_mutex_t* mutex) {
    mutex->value = 0;
}

int pthread_mutex_lock_ts(my_mutex_t* mutex) {
    my_test_and_set(&(mutex->value));
}

int pthread_mutex_lock_tts(my_mutex_t* mutex) {
    while (my_test_and_set(&(mutex->value))) {
        while (mutex->value) {}
    }
}

int pthread_mutex_unlock(my_mutex_t* mutex) {
    int *val = &(mutex->value);
    __asm__(
        "movl $0, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        :"+m" (*val)
        :
        :"%eax"
    );
}

int sem_init(my_sem_t* sem, int val) {
    sem->value = val;
    sem->wakeups = 0;
    pthread_mutex_init(sem->mutex);
}

int my_sem_wait(my_sem_t* sem) {
    pthread_mutex_lock_ts(sem->mutex);
    while (sem->value <= 0) {
        pthread_mutex_unlock(sem->mutex);
        pthread_mutex_lock_ts(sem->mutex);
    }
    sem->value--;
    pthread_mutex_unlock(sem->mutex);
}

int my_sem_post(my_sem_t *sem) {
    pthread_mutex_lock_ts(sem->mutex);
    sem->value++;
    pthread_mutex_unlock(sem->mutex);
}

int main() {
    my_mutex_t mutx;
    pthread_mutex_init(&mutx);
    pthread_mutex_lock_ts(&mutx);
    printf("i = %d\n", mutx.value);
    pthread_mutex_unlock(&mutx);
    printf("i = %d\n", mutx.value);
}