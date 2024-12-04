#include<stdio.h>
#include <string.h>
#include <math.h>
#include <cblas.h>
#include "Doc.c"
#include "tcorpus.c"

int Vocab(Dict2* tcorpus, char result[][25]) {
    char txt[1000] = "";
    int lng_res = 0;


    while (tcorpus != NULL) {
        Dict* current_dict = tcorpus->dictionaire;

   
        while (current_dict != NULL) {
            if (strlen(txt) + strlen(current_dict->mot) + 1 < sizeof(txt)) {
                strcat(txt, current_dict->mot);
                strcat(txt, " "); 
            }
            current_dict = current_dict->suivant;
        }
        tcorpus = tcorpus->suivant;
    }
    // Extraire les mots uniques
    lng_res = extract_unique_words(txt, NULL, 0, result);

    return lng_res;
}
int taille_de_dict(Dict* dic){
    int i=0;
    while(dic!=NULL){
        i++;
        dic=dic->suivant;
    }
    return i;
}
int taille_de_dict2(Dict2* tcorpus){
    int i=0;
    while(tcorpus!=NULL){
        i++;
        tcorpus=tcorpus->suivant;
    }
    return i;
}
int tf_mat(Dict2* tcorpus, float mat[taille_de_dict2(tcorpus)][taille_de_dict(tcorpus->dictionaire)]) {
    int i = 0, j=0; // Initialize counters
    Dict* p;
    char skill[25];
    while (tcorpus != NULL) {
        strcpy(skill, tcorpus->mot);;
        p = tcorpus->dictionaire;
        j = 0;
        do{
        mat[i][j]=tf(tcorpus,p->mot,skill);
        j++;
        p =p->suivant;
        }while(p!=NULL);
        tcorpus->suivant;
        i++; 
    }
    return i * j;
}
int idf(Dict2* tcorpus,char* word[25]){
    int res=-1;
    int nbrskill=0;
    int nbrword=0;
    Dict* dic=tcorpus->dictionaire;
    while(tcorpus!=NULL){
        nbrskill++;
        tcorpus=tcorpus->suivant;
    }
    while(dic!=NULL){
        if(strcmp(dic->mot, *word) == 0){
            nbrword++;
        }
        dic=dic->suivant;
    }
    if(nbrword!=0){
            res=log10(nbrskill/nbrword);
    }
    return res;
}
int idf_vect(Dict2* tcorpus,int res[],char vocab[][25]){
    int** p=vocab;
    int l,i=0;
    l=Vocab(tcorpus, vocab);
    while(i<=l){
        res[i]=idf(tcorpus,p);
        i++;
        p++;
    }
    return l;
}
float tf(Dict2* tcorpus ,char word[25],char skill[25]){
    int lgn;
    char result[100][25];
    lgn=Vocab(tcorpus,result);
    while(tcorpus!=NULL){
        if(strcmp(tcorpus->mot, skill) == 0){
            do{
                if(strcmp(tcorpus->dictionaire->mot, word) == 0){
                    return (1/lgn);
                }
                tcorpus->dictionaire=tcorpus->dictionaire->suivant;
            }while(tcorpus->dictionaire!=NULL);
        }
        tcorpus=tcorpus->suivant;
    }
    return 0;
}
int tf_idf_mat(Dict2* tcorpus,int idf_vect[taille_de_dict(tcorpus->dictionaire)],float mat[taille_de_dict2(tcorpus)][taille_de_dict(tcorpus->dictionaire)],float res[taille_de_dict2(tcorpus)][taille_de_dict(tcorpus->dictionaire)]){
    int j=0,i=0;
    while (i<=taille_de_dict2(tcorpus)){
        while(j<=taille_de_dict(tcorpus->dictionaire)){
            res[i][j]=idf_vect[j]*mat[i][j];
            j++;
        }
        i++;
    }
    return 0;
}