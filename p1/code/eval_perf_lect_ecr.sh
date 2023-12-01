#!/bin/bash
TOMEASURE=code/lecteurs_ecrivains
OUT=$TOMEASURE.csv
NMes=5


echo "nombre de coeurs,mesure" > $OUT
for n in 2 4 8 16 32 64; do
    for i in $(seq 1 $NMes); do
        echo -n "$n," >> $OUT
        /usr/bin/time -f "%e" -o $OUT -a ./$TOMEASURE $(($n/2)) $(($n/2))
    done
done