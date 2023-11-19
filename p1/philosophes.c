#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


int philo;
pthread_t phil;
pthread_mutex_t* baguette;

void mange(int id) {
printf("Philosophe [%d] mange\n",id);

}

void* philosophe ( void* arg )
{
int *id=(int *) arg;
int left = *id;
int right = (left + 1) % philo;

if(left<right) {
pthread_mutex_lock(&baguette[left]);
pthread_mutex_lock(&baguette[right]);
}

else {
    pthread_mutex_lock(&baguette[right]);
    pthread_mutex_lock(&baguette[left]);
}

mange(*id);
pthread_mutex_unlock(&baguette[left]);
pthread_mutex_unlock(&baguette[right]);
return (NULL);
}


int main(int argc, char *argv[]) {
    philo = atoi(argv[1]);
    baguette = (pthread_mutex_t*)malloc(philo* sizeof(pthread_mutex_t));

    //init mutex for each baguette
     for(int i= 0; i<philo; i++){
        pthread_mutex_init(&baguette[i], NULL);
    }
    
    int id[philo];
    /*
    int C0=0;
    int c1=0;
    int C2=0;
    */
    for(int j=0; j<10000000  ; j++){
        for(int i= 0; i<philo; i++){
            id[i]= i;
            pthread_create(&phil, NULL, philosophe, &id[i]);
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
    }
}
