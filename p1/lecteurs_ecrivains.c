#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include <semaphore.h>

pthread_mutex_t mutex_read;
pthread_mutex_t mutex_write;
pthread_mutex_t z;
sem_t sem_write;
sem_t sem_read;
int readcount=0;
int writecount=0;


void* writer(void* arg) {
    for(int i = 0; i < 640; i++) {
        //prepare_data();
        
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
   return NULL;
}


void* reader(void* arg) {
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

        //process_data();
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    sem_init(&sem_write, 0, 1);
    sem_init(&sem_read, 0, 1);

    //int nWriters = atoi(argv[1]);
    //int nReaders = atoi(argv[2]);

    int nWriters = 10;
    int nReaders = 10;

    pthread_t* writers = malloc(nWriters * sizeof(pthread_t));
    pthread_t* readers = malloc(nReaders * sizeof(pthread_t));
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
}