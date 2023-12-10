#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "my_mutex.h"


int nPhilo;
my_mutex_t* baguette;

void mange(int id) {
printf("Philosophe [%d] mange\n",id);
}

void* philosophe ( void* arg ) {
    for (int i = 0; i < 1000000; i++) {
        int *id=(int *) arg;
        int left = *id;
        int right = (left + 1) % nPhilo;
 
        if(right<left) {
            int tmp = left;
            left = right;
            right = tmp;
        } // On fait en sorte que la baguette de gauche soit toujours la plus petite

        my_mutex_lock_tts(&baguette[left]);
        my_mutex_lock_tts(&baguette[right]);

        //mange(*id);

        my_mutex_unlock(&baguette[left]);
        my_mutex_unlock(&baguette[right]);
    }
    return (NULL);
}   


int main(int argc, char *argv[]) {
    nPhilo = atoi(argv[1]);
    baguette = (my_mutex_t*)malloc(nPhilo* sizeof(my_mutex_t));
    pthread_t phils[nPhilo];
    int id[nPhilo];

    //init mutex for each baguette
    for(int i= 0; i<nPhilo; i++){
        my_mutex_init(&baguette[i]);
    }
    
    /*
    int C0=0;
    int c1=0;
    int C2=0;
    */
    for(int i= 0; i<nPhilo; i++){
        id[i]= i;
        pthread_create(phils + i, NULL, &philosophe, id + i);
        /*
        if(i==0){
            C0++;
        }
        if(i==1){
            c1++;
        }
        if(i==2){
            C2++;
        }
        */
    }
    void* res;
    for (int i = 0; i < nPhilo; i++) {
        pthread_join(phils[i], &res);
    }
}
