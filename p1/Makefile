#Makefile pour les test du P1

#all: compile tous les fichiers nécessaires à l'exécution de tous les tests
#tests: lance tous les tests
#clean: clean tous les fichiers créés par make all et make tests

#Partie 1:
#run1: compile les fichier c pour les 3 problèmes avec l'implementation des mutex POSIX
#test1: test les 3 problèmes et stock les fichiers CSV dans le fichier code  (philosophes prend une bonne dizaine de minutes)
#clean1: clean les fichiers pour la partie 1

#Partie 2_ts:
#run_ts: compile les 3 problèmes avec l'implementation Test-and-Set
#test_ts: test les 3 problèmes et stock les fichiers CSV dans le fichier Mutex_ts (le prod_cons prend une bonne dizaine de minutes)
#clean_ts: clean les fichier pour la partie 2_ts 

#Partie 2_tts:
#run_tts: compile les 3 problèmes avec l'implementation Test-and-Test-and-Set
#test_tts: test les 3 problèmes et stock les fichiers CSV dans le fichier tache 2 (philosophes prend une bonne dizaine de minutes)
#clean_tts: clean les fichier pour la partie 2_tts 

all:
	make run1
	make run_ts
	make run_tts

tests:
	make test1
	make test_ts
	make test_tts

clean:
	rm philosophes
	rm philosophes.o
	rm producteurs_consommateurs
	rm producteurs_consommateurs.o
	rm lecteurs_ecrivains
	rm lecteurs_ecrivains.o
	rm my_mutex.o
	make clean1
	make clean_ts
	make clean_tts

CC = gcc
LIBS = -lpthread
INCLUDE = tache2/my_mutex.h
#ARGS Tache 1

ARGS1A = code/lecteurs_ecrivains
ARGS1B = code/philosophes
ARGS1C = code/producteurs_consommateurs


#Test 1
FILE1A = code/eval_perf_lect_ecr.sh
FILE1B = code/eval_perf_philosophes.sh
FILE1C = code/eval_perf_prod_cons.sh



run1:
	$(CC) -I/$(INCLUDE) -o $(ARGS1A) $(ARGS1A).c $(LIBS)
	$(CC) -I/$(INCLUDE) -o $(ARGS1C) $(ARGS1C).c $(LIBS)
	$(CC) -I/$(INCLUDE) -o $(ARGS1B) $(ARGS1B).c $(LIBS)


test1:
	sh $(FILE1A)
	sh $(FILE1C)
	sh $(FILE1B)


clean1:
	rm $(ARGS1A)
	rm $(ARGS1B)
	rm $(ARGS1C)
	rm code/producteurs_consommateurs.csv
	rm code/philosophes.csv


#ARGS Tache 2 TTS

ARGS2A = lecteurs_ecrivains
ARGS2B = philosophes
ARGS2C = producteurs_consommateurs

ANS2A = lecteurs_ecrivains
ANS2B = philosophes
ANS2C = producteurs_consommateurs

STRUCT= my_mutex
SOURCE = Mutex_ts


run_tts:
	$(CC) $(LIBS)  -c $(STRUCT).c 
	$(CC) $(LIBS)  -c $(ARGS2A).c 
	$(CC) $(LIBS) $(STRUCT).o $(ARGS2A).o -o $(ANS2A)

	$(CC) $(LIBS)  -c $(ARGS2B).c 
	$(CC) $(LIBS) $(STRUCT).o $(ARGS2B).o -o $(ANS2B)

	$(CC) $(LIBS)  -c $(ARGS2C).c 
	$(CC) $(LIBS) $(STRUCT).o $(ARGS2C).o -o $(ANS2C)




clean_tts:
	rm $(STRUCT).o
	rm $(ARGS2A).o
	rm $(ANS2A)
	rm tache2/$(ANS2A).csv
	rm $(ANS2B)
	rm tache2/$(ANS2B).csv
	rm $(ANS2C)
	rm tache2/$(ANS2C).csv
	

test_tts:
	sh eval_lect_ecr.sh
	sh eval_prod_cons.sh
	sh eval_philo.sh

#ARGS tache 2 TS
ARGS3A = lecteurs_ecrivains
ARGS3B = philosophes
ARGS3C = producteurs_consommateurs

ANS3A = lecteurs_ecrivains
ANS3B = philosophes
ANS3C = producteurs_consommateurs

STRUCT= my_mutex

run_ts:
	$(CC) $(LIBS)  -c Mutex_ts/$(STRUCT).c 
	$(CC) $(LIBS)  -c Mutex_ts/$(ARGS3C).c 
	$(CC) $(LIBS) $(STRUCT).o $(ARGS3C).o -o $(ANS3C)
	
	$(CC) $(LIBS)  -c Mutex_ts/$(ARGS3A).c 
	$(CC) $(LIBS) $(STRUCT).o $(ARGS3A).o -o $(ANS3A)
	
	$(CC) $(LIBS)  -c Mutex_ts/$(ARGS3B).c 
	$(CC) $(LIBS) $(STRUCT).o $(ARGS3B).o -o $(ANS3B)

test_ts:
	sh Mutex_ts/eval_mymut_lect.sh
	sh Mutex_ts/eval_mymut_philo.sh
	sh Mutex_ts/eval_mymut_prod.sh

clean_ts:
	rm $(STRUCT).o
	rm $(ARGS3A).o
	rm $(ANS3A)
	rm Mutex_ts/$(ANS3A).csv
	rm $(ARGS3C).o
	rm $(ANS3C)
	rm Mutex_ts/$(ANS3C).csv
	rm $(ARGS3B).o
	rm $(ANS3B)
	rm Mutex_ts/$(ANS3B).csv




#$(CC) -I/$(INCLUDE) -o $(ARGS) $(ARGS).c $(LIBS)
