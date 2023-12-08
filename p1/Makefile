CC = gcc
LIBS = -lpthread
INCLUDE = tache2/my_mutex.h
#ARGS Tache 1

#ARGS = code/lecteurs_ecrivains
#ARGS = code/philosophes
#ARGS = code/producteurs_consommateurs

#ARGS Tache 2 TS

#ARGS = Mutex_ts/lecteurs_ecrivains
ARGS = philosophes
#ARGS = producteurs_consommateurs
STRUCT= my_mutex

#ARGS = tache2/my_mutex

run:
	$(CC) $(LIBS) -c $(STRUCT).c 
	$(CC) $(LIBS) -c $(ARGS).c 

	$(CC)  $(LIBS) $(STRUCT).o  $(ARGS).o -o philo

clean:
	rm $(STRUCT).o
	rm $(ARGS).o


#$(CC) -I/$(INCLUDE) -o $(ARGS) $(ARGS).c $(LIBS)
