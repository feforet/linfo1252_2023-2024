#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include "my_mutex.h"

//Mymut_ts

#define BUF_SIZE 8
#define N_DATA 8192


my_mutex_t mutex;

my_sem_t empty;
my_sem_t full;
/*
sem_t empty;
sem_t full;
*/

int buffer[BUF_SIZE];
int x = 0;
int y = 0;

void* producer () {
    while (true) {
        for (int i=0; i<10000; i++){};
        int toWrite = 1;
        my_sem_wait(&empty);
        my_mutex_lock_ts(&mutex);
        if (x == N_DATA) {
            my_mutex_unlock(&mutex);
            my_sem_post(&full);
            break;

        }
        //insert_item();
        /*
        if (buffer[x % BUF_SIZE] != 0) {
            printf("x pass insert\n");
            pthread_mutex_unlock(&mutex);
            sem_post(&full);
            //sem_post(&empty);//why
            return (void *)-1;
        }
        */
        buffer[x % BUF_SIZE] = toWrite;
        x++;
        my_mutex_unlock(&mutex);
        my_sem_post(&full);
    }
    return (void *)0;
}

void* consumer () {
    int res;
    while (true) {
        my_sem_wait(&full);
        my_mutex_lock_ts(&mutex);
        if (y == N_DATA) {
            my_mutex_unlock(&mutex);
            
            my_sem_post(&empty);
            my_sem_post(&full);
            break;
        }
        //remove_item();
        //y++;
        res = buffer[y % BUF_SIZE];
        y++;
        /*
        if (res == 0) {
            printf("y passes res \n");
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            //sem_post(&full);//NOO
            return (void *)-1;

        }
        */
        my_mutex_unlock(&mutex);
        my_sem_post(&empty);
        for (int i=0; i<10000; i++){};
    }
    return (void *) 0;
}

int main(int argc, char *argv[]) {
    int nCons = atoi(argv[1]);
    int nProd = atoi(argv[2]);
    my_mutex_init(&mutex);
    
    my_sem_init(&empty, 0);
    my_sem_init(&full, 0);
    
    

    pthread_t producers[nProd];
    pthread_t consumers[nCons];
    for (int i = 0; i < nProd; i++) {
        pthread_create(producers + i, NULL, &producer, NULL);
    }
    for (int i = 0; i < nCons; i++) {
        pthread_create(consumers + i, NULL, &consumer, NULL);
    }

    void* res;
    int err = 0;
    for (int i = 0; i < nProd; i++) {
        pthread_join(producers[i], &res);
        if (((int*) res) != 0) err = -1;
    }
    for (int i = 0; i < nCons; i++) {
        pthread_join(consumers[i], &res);
        if (((int*) res) != 0) err = -1;
    }

    //pthread_mutex_destroy(&mutex);
    //sem_destroy(&empty);
    //sem_destroy(&full);
    return err;
}