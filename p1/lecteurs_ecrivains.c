#include <stdint.h>
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


void writer(void) {
    while(true) {
        //prepare_data();
        
        pthread_mutex_lock(&mutex_write);
        writecount++;
        if (writecount==1) sem_wait(&sem_read);
        pthread_mutex_unlock(&mutex_write);
        
        sem_wait(&sem_write);
        // section critique
        //write_database();
        sem_post(&sem_write);
        
        pthread_mutex_lock(&mutex_write);
        writecount--;
        if (writecount==0) sem_post(&sem_read);
        pthread_mutex_unlock(&mutex_write);
   }
}


void reader(void) {
    while(true) {
        pthread_mutex_lock(&z); // un seul reader en attente sur rsem
        sem_wait(&sem_read);
        pthread_mutex_lock(&mutex_read);
        readcount++;
        if (readcount==1) sem_wait(&sem_write);
        // section critique
        //read_database();
        //
    }
}


int main(int argc, char *argv[]) {
    sem_init(&sem_write, 0, 1);
    sem_init(&sem_read, 0, 1);
    printf("blabla");
}