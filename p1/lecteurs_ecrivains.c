#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include "my_mutex.h"

my_mutex_t mutex_write;
my_mutex_t mutex_read;
my_mutex_t z;
my_sem_t sem_write;
my_sem_t sem_read;
int writecount=0;
int readcount=0;
int ecrits=0;
int lus=0;

// il faut faire 640 ecritures et 2560 lectures AU TOTAL et pas chacun
void* writer() {
    while(true) {
        my_mutex_lock_tts(&mutex_write);
        if (++ecrits > 640){
            my_mutex_unlock(&mutex_write);
            break;
        }
        writecount++;
        if (writecount==1) my_sem_wait(&sem_read);
        my_mutex_unlock(&mutex_write);
        
        my_sem_wait(&sem_write);
        // section critique
        for (int i=0; i<10000; i++);
        my_sem_post(&sem_write);
        
        my_mutex_lock_tts(&mutex_write);
        writecount--;
        if (writecount==0) my_sem_post(&sem_read);
        my_mutex_unlock(&mutex_write);
    }
    return 0;
}


void* reader() {
    while(true) {
        my_mutex_lock_tts(&z); // un seul reader en attente sur sem_read
        my_sem_wait(&sem_read);
        my_mutex_lock_tts(&mutex_read);
        if (++lus > 2560) {
            my_mutex_unlock(&z);
            my_sem_post(&sem_read);
            my_mutex_unlock(&mutex_read);
            break;
        }
        readcount++;
        if (readcount==1) my_sem_wait(&sem_write);
        my_mutex_unlock(&mutex_read);
        my_sem_post(&sem_read);
        my_mutex_unlock(&z);

        // section critique
        for (int i=0; i<10000; i++);

        my_mutex_lock_tts(&mutex_read);
        readcount--;
        if(readcount==0) my_sem_post(&sem_write);
        my_mutex_unlock(&mutex_read);
    }
    return 0;
}


int main(int argc, char *argv[]) {
    int nWriters = atoi(argv[1]);
    int nReaders = atoi(argv[2]);

    my_sem_init(&sem_write, 1);
    my_sem_init(&sem_read, 1);
    my_mutex_init(&z);
    my_mutex_init(&mutex_write);
    my_mutex_init(&mutex_read);

    pthread_t writers[nWriters];
    pthread_t readers[nReaders];
    for (int i = 0; i < nWriters; i++) {
        pthread_create(writers + i, NULL, &writer, NULL);
    }
    for (int i = 0; i < nReaders; i++) {
        pthread_create(readers + i, NULL, &reader, NULL);
    }

    void* res;
    for (int i = 0; i < nWriters; i++) {
        pthread_join(writers[i], &res);
    }
    for (int i = 0; i < nReaders; i++) {
        pthread_join(readers[i], &res);
    }
/*
    pthread_mutex_destroy(&mutex_write);
    pthread_mutex_destroy(&mutex_read);
    pthread_mutex_destroy(&z);
    sem_destroy(&sem_write);
    sem_destroy(&sem_read);
    */
}