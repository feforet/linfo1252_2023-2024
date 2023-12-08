#!/bin/bash
TOMEASURE=philo
OUT=$TOMEASURE.csv
NMes=5


echo "nombre de coeurs,mesure" > $OUT
for n in  2 4 8 16; do
    for i in $(seq 1 $NMes); do
        echo -n "$n," >> $OUT
        /usr/bin/time -f "%e" -o $OUT -a ./$TOMEASURE $n 
    done
done

cat $OUT