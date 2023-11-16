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
}

void init_2_chainee() {
    uint16_t *HEAP = (uint16_t*) MY_HEAP;
    //initialiser le 1e bloc de m-d a 'libre'
    uint16_t sizeOfTheBlock = 32000 - 2;
    uint16_t sizeOfPrec = 0;
    HEAP[0] = sizeOfTheBlock;
    HEAP[1] = sizeOfPrec;
}

void my_free(void *pointer) {
    //set les m-d du bloc a 'libre'
    uint8_t *md_pointer = ((uint8_t*) pointer) - 3;
    *md_pointer = 0;

    // fusionner le bloc libere avec le suivant si il est libre
    uint16_t sizeOfFree = ((((uint16_t) *(md_pointer+1)) << 8) + ((uint16_t) *(md_pointer+2)));
    uint8_t *nextMD = (md_pointer + 3 + sizeOfFree);
    if ((nextMD < &(MY_HEAP[0]) + 64000) && *(nextMD) == 0) {
        uint16_t sizeOfNext = ((((uint16_t) *(nextMD+1)) << 8) + ((uint16_t) *(nextMD+2)));
        uint16_t newSize = sizeOfFree + 3 + sizeOfNext;
        *(md_pointer + 1) = (newSize >> 8) & 0b11111111;
        *(md_pointer + 2) = newSize & 0b11111111;
    }
}

void my_free_2_chainee(void *pointer) {
    uint16_t *ptr_current = ((uint16_t*) pointer) - 2;
    uint16_t sizeOfCurrent = *ptr_current & 0b0111111111111111;
    uint16_t sizeOfPrec = *(ptr_current + 1) & 0b0111111111111111;
    uint16_t *ptr_next = ptr_current + 2 + sizeOfCurrent;
    uint16_t *ptr_prec = ptr_current - sizeOfPrec - 2;
    //set le bloc a libre
    *ptr_current = *ptr_current & 0b0111111111111111;
    //fusionner le bloc avec le suivant
    if ((ptr_next < (uint16_t*) (MY_HEAP + 64000)) && (*ptr_next >> 15) == 0) {
        uint16_t sizeOfNext = *ptr_next & 0b0111111111111111;
        sizeOfCurrent = sizeOfCurrent + 2 + sizeOfNext;
        *ptr_current = sizeOfCurrent;
    }
    //fusionner le bloc avec le precedent
    if ((ptr_prec >= (uint16_t*) MY_HEAP) && (*ptr_prec >> 15) == 0) {
        uint16_t sizeOfPrec = *ptr_prec & 0b0111111111111111;
        *ptr_prec = sizeOfPrec + 2 + sizeOfCurrent;
    }
}

void *my_malloc(size_t size) {
    //parcourir la heap de m-d en m-d jusqu'a trouver un bloc de taille >= sizeWeNeed
    uint16_t sizeWeNeed = size;
    int current = 0;
    while (current < 64000) {
        // verification de la disponibilite et de la taille
        uint16_t sizeOfCurrent = ((((uint16_t) MY_HEAP[current+1]) << 8) + ((uint16_t) MY_HEAP[current+2]));
        if ((MY_HEAP[current] == 0) && (sizeOfCurrent >= sizeWeNeed)) {
            //set les m-d du bloc a 'occupe'
            MY_HEAP[current] = 1;

            //si sizeOfCurrent >> sizeWeNeed: créer des m-d a heap[bloc + sizeWeNeed]
            //le +4 permet de s'assurer que on aura la place pour stocker au moins 1 octet 
            //apres avoir place 3 blocs de m-d
            if ((sizeWeNeed + 4) < sizeOfCurrent) {
                // set un nouveau bloc de m-d à HEAP[current+sizeWeNeed+3]
                int newMeta = current + 3 + sizeWeNeed;
                uint16_t sizeOfNew = sizeOfCurrent - sizeWeNeed - 3;
                MY_HEAP[newMeta] = 0;
                MY_HEAP[newMeta + 1] = (sizeOfNew >> 8) & 0b11111111;
                MY_HEAP[newMeta + 2] = sizeOfNew & 0b11111111;

                // reset la taille du bloc actuel a sizeWeNeed
                MY_HEAP[current + 1] = (sizeWeNeed >> 8) & 0b11111111;
                MY_HEAP[current + 2] = sizeWeNeed & 0b11111111;
            }
            //return heap[bloc + sizeof(meta-donnees)]
            return &(MY_HEAP[current+3]);
        } else {
            current += 3 + sizeOfCurrent;
        }
    }
    return NULL; // si pas de place dans la stack (tout est pris ou pas de place assez grande)
}

void *my_malloc_2_chainee(size_t size) {
    uint16_t *HEAP = (uint16_t*) MY_HEAP;
    //parcourir la heap de m-d en m-d jusqu'a trouver un bloc de taille >= sizeWeNeed
    uint16_t sizeWeNeed = (size + (size % 2)) / 2;  //arrondit au pair superieur
    int current = 0;
    while (current < 32000) {
        // verification de la disponibilite et de la taille
        uint16_t sizeOfCurrent = HEAP[current] & 0b0111111111111111;
        if (((HEAP[current] >> 15) == 0) && (sizeOfCurrent >= sizeWeNeed)) {
            //si on alloue dans un bloc trop grand, on le separe en 2
            if (sizeOfCurrent > sizeWeNeed + 3) {
                HEAP[current] = sizeWeNeed;
                //creer un nouveau bloc de metadonnees et l'initialiser
                int newMeta = current + 2 + sizeWeNeed;
                uint16_t sizeOfNew = sizeOfCurrent - sizeWeNeed - 2;
                HEAP[newMeta] = sizeOfNew;
                HEAP[newMeta + 1] = sizeWeNeed;
                // changer la taille prec du suivant
                if (current + sizeOfCurrent + 3 < 32000) {
                    HEAP[current + sizeOfCurrent + 3] = sizeOfNew;
                }
            }

            //set les m-d du bloc a 'occupe'
            HEAP[current] = HEAP[current] | (uint16_t) 1 << 15;

            return &(HEAP[current + 2]);
        } else {
            current += 2 + sizeOfCurrent;
        }
    }
    return NULL; // si pas de place dans la stack
}

void *my_malloc_meilleure_recherche(size_t size) {
    uint16_t *HEAP = (uint16_t*) MY_HEAP;
    //parcourir la heap de m-d en m-d jusqu'a trouver un bloc de taille >= sizeWeNeed
    uint16_t sizeWeNeed = (size + (size % 2)) / 2;  //arrondit au pair superieur
    int current = 0;
    while (current < 32000) {
        // verification de la disponibilite et de la taille
        uint16_t sizeOfCurrent = HEAP[current] & 0b0111111111111111;
        if (((HEAP[current] >> 15) == 0) && (sizeOfCurrent >= sizeWeNeed)) {
            //si on alloue dans un bloc trop grand, on le separe en 2
            if (sizeOfCurrent > sizeWeNeed + 3) {
                HEAP[current] = sizeWeNeed;
                //creer un nouveau bloc de metadonnees et l'initialiser
                int newMeta = current + 2 + sizeWeNeed;
                uint16_t sizeOfNew = sizeOfCurrent - sizeWeNeed - 2;
                HEAP[newMeta] = sizeOfNew;
                HEAP[newMeta + 1] = sizeWeNeed;
                // changer la taille prec du suivant
                if (current + sizeOfCurrent + 3 < 32000) {
                    HEAP[current + sizeOfCurrent + 3] = sizeOfNew;
                }
            }

            //set les m-d du bloc a 'occupe'
            HEAP[current] = HEAP[current] | (uint16_t) 1 << 15;

            return &(HEAP[current + 2]);
        } else {
            current += 2 + sizeOfCurrent;
        }
    }
    return NULL; // si pas de place dans la stack
}


void print_heap(int n) {
    uint16_t* HEAP = (uint16_t*) MY_HEAP;
    for (int i = 0; i < n; i++){
        printf("%b  \t", HEAP[i]);
    }
    printf("\n\n");
}

int main(int argc, char *argv[]) {
    int a = 13;
    init_2_chainee();
    print_heap(a);
    uint8_t* ptr1 = (uint8_t*) my_malloc_2_chainee(1);
    print_heap(a);
    *ptr1 = 10;
    print_heap(a);
    uint8_t *ptr2 = (uint8_t*) my_malloc_2_chainee(2);
    print_heap(a);
    *(ptr2+1) = 10;
    print_heap(a);
    my_free_2_chainee(ptr1);
    print_heap(a);

    return 0;
}
