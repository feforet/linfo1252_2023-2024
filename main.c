#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t MY_HEAP[64000];

int main(int argc, char *argv[]) {
    return 0;
}

void init() {
    //initialiser le 1e bloc de m-d a 'libre'
}

void my_free(void *pointer) {
    //set les m-d du bloc a 'libre'
    // si on sait, augmenter la taille du bloc
    //    en y ajoutant la taille du bloc d'apres,
    //    pour eviter le partitionnement
}
void my_malloc(size_t size) {
    //parcourir la heap de m-d en m-d jusqu'a trouver un bloc
    //de taille >= size
    //set les m-d du bloc a 'occupe'
    //si taille >= size: créer des m-d a heap[bloc + size]
    //return heap[bloc + sizeof(meta-donnees)]
}