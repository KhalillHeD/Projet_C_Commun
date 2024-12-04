#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dict Dict;
struct Dict {
    char mot[25];
    float freq;
    Dict *suivant;
};

Dict* initialisation1(const char array[], float freqe) {
    Dict *liste = malloc(sizeof(*liste));
    if (liste == NULL) {
        exit(EXIT_FAILURE);
    }

    strncpy(liste->mot, array, sizeof(liste->mot) - 1);
    liste->mot[sizeof(liste->mot) - 1] = '\0'; // Assurer la terminaison
    liste->freq = freqe;
    liste->suivant = NULL;

    return liste;
}

void insertion(Dict *liste, const char *nmot, float freqe) {
    Dict *nouveau = malloc(sizeof(*nouveau));
    if (nouveau == NULL) {
        exit(EXIT_FAILURE);
    }

    strncpy(nouveau->mot, nmot, sizeof(nouveau->mot) - 1);
    nouveau->mot[sizeof(nouveau->mot) - 1] = '\0';
    nouveau->freq = freqe;
    nouveau->suivant = liste->suivant;
    liste->suivant = nouveau;
}

void suppression(Dict **liste) {
    if (*liste == NULL) {
        return;
    }

    Dict *aSupprimer = *liste;
    *liste = (*liste)->suivant;
    free(aSupprimer);
}

void afficher(Dict *liste) {
    while (liste != NULL) {
        printf("Mot: %s, Freq: %d\n", liste->mot, liste->freq);
        liste = liste->suivant;
    }
}
//un dictionnaire qui contient les frequence des mots
