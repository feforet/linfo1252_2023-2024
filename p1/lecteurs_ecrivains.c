#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex_write;
pthread_mutex_t mutex_read;
pthread_mutex_t z;
sem_t sem_write;
sem_t sem_read;
int writecount=0;
int readcount=0;

// il faut faire 640 ecritures et 2560 lectures AU TOTAL et pas chacun
void* writer() {
    for(int i = 0; i < 640; i++) {
        pthread_mutex_lock(&mutex_write);
        writecount++;
        if (writecount==1) sem_wait(&sem_read);
        pthread_mutex_unlock(&mutex_write);
        
        sem_wait(&sem_write);
        // section critique
        for (int i=0; i<10000; i++);
        sem_post(&sem_write);
        
        pthread_mutex_lock(&mutex_write);
        writecount--;
        if (writecount==0) sem_post(&sem_read);
        pthread_mutex_unlock(&mutex_write);
    }
}


void* reader() {
    for(int i = 0; i < 2560; i++) {
        pthread_mutex_lock(&z); // un seul reader en attente sur sem_read
        sem_wait(&sem_read);
        pthread_mutex_lock(&mutex_read);
        readcount++;
        if (readcount==1) sem_wait(&sem_write);
        pthread_mutex_unlock(&mutex_read);
        sem_post(&sem_read);
        pthread_mutex_unlock(&z);

        // section critique
        for (int i=0; i<10000; i++);

        pthread_mutex_lock(&mutex_read);
        readcount--;
        if(readcount==0) sem_post(&sem_write);
        pthread_mutex_unlock(&mutex_read);
    }
}


int main(int argc, char *argv[]) {
    //int nWriters = atoi(argv[1]);
    //int nReaders = atoi(argv[2]);
    int nWriters = 10;
    int nReaders = 10;

    sem_init(&sem_write, 0, 1);
    sem_init(&sem_read, 0, 1);
    pthread_mutex_init(&z, NULL);
    pthread_mutex_init(&mutex_write, NULL);
    pthread_mutex_init(&mutex_read, NULL);

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
    
    printf("fini !!\n");

    pthread_mutex_destroy(&mutex_write);
    pthread_mutex_destroy(&mutex_read);
    pthread_mutex_destroy(&z);
    sem_destroy(&sem_write);
    sem_destroy(&sem_read);
}