#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

#define BUF_SIZE 8
#define N_DATA 8192


pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int buffer[BUF_SIZE];
int x = 0;
int y = 0;

void* producer () {
    while (true) {
        printf("x is: %i\n", x);
        //printf(" data = %i\n", N_DATA);
        for (int i=0; i<10000; i++);
        int toWrite = 1;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        if (x == N_DATA) {
            printf("x pass break\n");
            pthread_mutex_unlock(&mutex);
            sem_post(&full);
            sem_post(&empty);
            break;

        }
        //insert_item();
        if (buffer[x % BUF_SIZE] != 0) {
            printf("x pass insert\n");
            pthread_mutex_unlock(&mutex);
            sem_post(&full);
            sem_post(&empty);
            return (void *)-1;
        }
        //x++;
        buffer[x % BUF_SIZE] = toWrite;
        x++;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    return (void *)0;
}

void* consumer () {
    int res;
    while (true) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        printf("y is: %i \n", y);
        if (y == N_DATA) {
            printf("equals N_DATA\n");
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            sem_post(&full);
            break;
        }
        //remove_item();
        y++;
        res = buffer[y % BUF_SIZE];
        //y++;
        if (res == 0) {
            printf("y passes res \n");
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            sem_post(&full);
            return (void *)-1;

        }
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        for (int i=0; i<10000; i++);
    }
    return (void *) 0;
}

int main(int argc, char *argv[]) {
    printf("NEW CALL TO MAIN\n" );
    int nCons = atoi(argv[1]); //"2");
    int nProd = atoi(argv[2]);
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUF_SIZE);
    sem_init(&full, 0, 0);
    

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

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return err;
}