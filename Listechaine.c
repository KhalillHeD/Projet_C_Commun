#include<stdio.h>
#include<stdlib.h>
#include <string.h>
typedef struct Element Element;
struct Element
{
    char mot[25];
    Element* suivant;
};

Element* initialisation1(const char array[], int freqe) {
    Element *liste = malloc(sizeof(*liste));
    if (liste == NULL) {
        exit(EXIT_FAILURE);
    }

    strncpy(liste->mot, array, sizeof(liste->mot) - 1);
    liste->mot[sizeof(liste->mot) - 1] = '\0';
    liste->suivant = NULL;

    return liste;
}

void insertion(Element *liste, char nmot[])
{
    Element *nouveau = malloc(sizeof(*nouveau));
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    strncpy(liste->mot, nmot, sizeof(liste->mot) - 1);
    liste->mot[sizeof(liste->mot) - 1] = '\0';
    nouveau->suivant = liste->suivant;
    liste->suivant = nouveau;
}
void suppression(Element *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (liste->suivant != NULL)
    {
        Element *aSupprimer = liste;
        liste->suivant = liste->suivant->suivant;
        free(aSupprimer);
    }
}

void occurence(Element *liste){
    Element *P=malloc(sizeof(*P));
    Element *eltcom=malloc(sizeof(*eltcom));
    Element *eltasup=malloc(sizeof(*eltasup));
    P=liste;
    while(P!=NULL){
        eltcom=P->suivant;
        while(eltcom!=NULL){
            if(eltcom->mot==P->mot){
                eltasup=eltcom;
                eltcom=eltcom->suivant;
                free(eltasup);
            }
            else{
                eltcom=eltcom->suivant;
            }
        }
        P=P->suivant;
    }
}
//liste chainée contient tous les mot de text 