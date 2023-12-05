CC = gcc
LIBS = -lpthread
INCLUDE = tache2/my_mutex.h
#ARGS Tache 1

#ARGS = code/lecteurs_ecrivains
#ARGS = code/philosophes
#ARGS = code/producteurs_consommateurs

#ARGS Tache 2 TS

#ARGS = Mutex_ts/lecteurs_ecrivains
#ARGS = Mutex_ts/philosophes
ARGS = producteurs_consommateurs
STRUCT= my_mutex

#ARGS = tache2/my_mutex

run:
	$(CC) -c $(STRUCT).c $(LIBS)
	$(CC) -c $(ARGS).c $(LIBS)

	$(CC)  my_mutex.o  producteurs_consommateurs.o -o prod_cons

clean:
	rm my_mutex.o
	rm producteurs_consommateurs.o


#$(CC) -I/$(INCLUDE) -o $(ARGS) $(ARGS).c $(LIBS)
