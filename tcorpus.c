#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include "Listechaine.c"
#include "Doc.c"


typedef struct Dict2 Dict2;
struct Dict2
{
    char mot[25];
    Dict* dictionaire;
    Dict2 *suivant;
};

Dict2* initialisation(const char array[],Dict* dic)
{
    Dict2 *liste = malloc(sizeof(*liste));
    Dict2 *element = malloc(sizeof(*element));

    if (liste == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }
    strncpy(liste->mot, array, sizeof(liste->mot) - 1);
    liste->mot[sizeof(liste->mot) - 1] = '\0';
    liste->dictionaire = dic;
    liste->suivant = NULL;
    return liste;
}
void insertion(Dict2 *liste,const char *nmot[],Dict* dic)
{
    Dict2 *nouveau = malloc(sizeof(*nouveau));
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    strncpy(liste->mot, nmot, sizeof(liste->mot) - 1);
    liste->mot[sizeof(liste->mot) - 1] = '\0';
    nouveau->dictionaire = dic;
    nouveau->suivant = liste->suivant;
    liste->suivant = nouveau;
}
void suppression(Dict2 *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (liste->suivant != NULL)
    {
        Dict2 *aSupprimer = liste;
        liste->suivant = liste->suivant->suivant;
        free(aSupprimer);
    }
}
void afficher1(Dict2 *liste) {
    while (liste != NULL) {
        printf("Mot: %s, Dict:", liste->mot);
        Dict* L;
        L=&liste->dictionaire; 
        afficher(L);
        liste = liste->suivant;
    }
}
int skills(Dict2* tcor, int lng,char array[][25]){
    int c=1;
    while(tcor!=NULL && c<=lng){
        strcpy(tcor->mot,array[c]);
        c++;
        tcor=tcor->suivant;
    }
    return c;
}

//le tcorpus contient les module(ou les themes) aves les mots qui appartient a ce theme avec leur frequence
