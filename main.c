#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t MY_HEAP[64000];

void init() {
    //initialiser le 1e bloc de m-d a 'libre'
    uint16_t sizeOfTheBlock = 63997;
    MY_HEAP[0] = 0;
    MY_HEAP[1] = (sizeOfTheBlock >> 8) & 0b11111111;
    MY_HEAP[2] = sizeOfTheBlock & 0b11111111;
    printf("%b\n%b\n%b\n", MY_HEAP[0], MY_HEAP[1], MY_HEAP[2]);
}

void my_free(void *pointer) {
    //set les m-d du bloc a 'libre'
    uint8_t *md_pointer = ((uint8_t*) pointer) - 3;
    *md_pointer = 0;

    // si on sait, augmenter la taille du bloc
    //    en y ajoutant la taille du bloc d'apres,
    //    pour eviter le partitionnement
}

void *my_malloc(size_t size) {
    //parcourir la heap de m-d en m-d jusqu'a trouver un bloc
    //de taille >= size
    int current = 0;
    while ((current < 64000) && (MY_HEAP[current] == 1)){
        /* verifier la taille */
    }
    
    
    //set les m-d du bloc a 'occupe'
    //si taille >= size: créer des m-d a heap[bloc + size]
    //return heap[bloc + sizeof(meta-donnees)]
}

int main(int argc, char *argv[]) {
    init();
    return 0;
}
