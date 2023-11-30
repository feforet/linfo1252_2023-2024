#ifndef MY_MUTEX
#define MY_MUTEX

typedef struct {
    int value;
}my_mutex_t;

typedef struct {
    int value;
    my_mutex_t* mutex;
}my_sem_t;


int my_mutex_init(my_mutex_t* mutex);
int my_mutex_lock_ts(my_mutex_t* mutex);
int my_mutex_lock_tts(my_mutex_t* mutex);
int my_mutex_unlock(my_mutex_t* mutex);
int my_sem_init(my_sem_t* sem, int val);
int my_sem_wait(my_sem_t* sem);
int my_sem_post(my_sem_t* sem);


#endif // MY_MUTEX