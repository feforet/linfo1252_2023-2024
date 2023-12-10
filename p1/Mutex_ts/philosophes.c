#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "my_mutex.h"


int nPhilo;
my_mutex_t* baguette;

void* philosophe ( void* arg ) {
    for (int i = 0; i < 1000000; i++) { //nombre de cycles réduit
        int *id=(int *) arg;
        int left = *id;
        int right = (left + 1) % nPhilo;

        if(right<left) {
            int tmp = left;
            left = right;
            right = tmp;
        } // On fait en sorte que la baguette de gauche soit toujours la plus petite

        my_mutex_lock_ts(&baguette[left]);
        my_mutex_lock_ts(&baguette[right]);

        // philosophe mange

        my_mutex_unlock(&baguette[left]);
        my_mutex_unlock(&baguette[right]);
    }
    return (NULL);
}   


int main(int argc, char *argv[]) {
    nPhilo = atoi(argv[1]);
    baguette = (my_mutex_t*) malloc(nPhilo * sizeof(my_mutex_t));
    pthread_t phils[nPhilo];
    int id[nPhilo];

    //init mutex for each baguette
    for(int i= 0; i<nPhilo; i++){
        my_mutex_init(&baguette[i]);
    }
    
    for(int i = 0; i<nPhilo; i++){
        id[i] = i;
        pthread_create(phils + i, NULL, &philosophe, id + i);
    }
    void* res;
    for (int i = 0; i < nPhilo; i++) {
        pthread_join(phils[i], &res);
    }
}
