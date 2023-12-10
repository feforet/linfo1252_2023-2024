#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


int nPhilo;
pthread_mutex_t* baguette;

void* philosophe ( void* arg ) {
    for (int i = 0; i < 10000000; i++) {
        int *id=(int *) arg;
        int left = *id;
        int right = (left + 1) % nPhilo;

        if(right<left) {
            int tmp = left;
            left = right;
            right = tmp;
        } // On fait en sorte que la baguette de gauche soit toujours la plus petite

        pthread_mutex_lock(&baguette[left]);
        pthread_mutex_lock(&baguette[right]);

        // philosophe mange

        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);
    }
    return (NULL);
}   


int main(int argc, char *argv[]) {
    nPhilo = atoi(argv[1]);
    baguette = (pthread_mutex_t*) malloc(nPhilo * sizeof(pthread_mutex_t));
    pthread_t phils[nPhilo];
    int id[nPhilo];

    //init mutex for each baguette
    for(int i= 0; i<nPhilo; i++){
        pthread_mutex_init(&baguette[i], NULL);
    }

    for(int i = 0; i<nPhilo; i++){
        id[i] = i;
        pthread_create(phils + i, NULL, &philosophe, id + i);
    }
    void* res;
    for (int i = 0; i < nPhilo; i++) {
        pthread_join(phils[i], &res);
    }
    for(int i = 0; i<nPhilo; i++){
        pthread_mutex_destroy(&baguette[i]);
    }
}
