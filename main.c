#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t MY_HEAP[64000];

int main(int argc, char *argv[]) {
    return 0;
}

void init() {
    //initialiser le 1e bloc de m-d à 'libre'
}

void my_free(void *pointer) {
    //set les m-d du bloc à 'libre'
    // si on sait, augmenter la taille du bloc
    //    en y ajoutant la taille du bloc d'apres,
    //    pour éviter le partitionnement
}
void my_malloc(size_t size) {
    //parcourir la heap de m-d en m-d jusqu'à trouver un bloc
    //de taille >= size
    //set les m-d du bloc à 'occupé'
    //si taille >= size: créer des m-d a heap[bloc + size]
    //return heap[bloc + sizeof(méta-données)]
}