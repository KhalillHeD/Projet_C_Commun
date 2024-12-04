#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// Définir une structure pour les documents traités
typedef struct {
    char term[25];
    double tf;
} Term;

typedef struct {
    Term* terms;
    int size;
} ProcessedDoc;

// Fonction 1 : Vérifier si un mot est alphabétique
bool is_alpha(const char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (!(word[i] >= 'a' && word[i] <= 'z')) {
            return false;
        }
    }
    return true;
}

// Fonction 2 : Prétraiter le texte
ProcessedDoc preprocess(const char* txt, char vocab[][25], int vocab_size) {
    char* copy = strdup(txt);
    char* token;
    int term_count = 0;
    Term terms[100]; // Limite temporaire pour le nombre de termes
    int word_count = 0;

    // Convertir le texte en minuscule
    for (int i = 0; copy[i]; i++) {
        copy[i] = tolower(copy[i]);
    }

    // Séparer le texte en mots
    token = strtok(copy, " ");
    while (token != NULL) {
        bool valid = true;

        if (vocab_size > 0) {
            valid = false;
            for (int i = 0; i < vocab_size; i++) {
                if (strcmp(token, vocab[i]) == 0) {
                    valid = true;
                    break;
                }
            }
        } else {
            valid = is_alpha(token);
        }

        if (valid) {
            bool found = false;
            for (int i = 0; i < term_count; i++) {
                if (strcmp(terms[i].term, token) == 0) {
                    found = true;
                    terms[i].tf += 1;
                    break;
                }
            }
            if (!found) {
                strcpy(terms[term_count].term, token);
                terms[term_count].tf = 1;
                term_count++;
            }
            word_count++;
        }

        token = strtok(NULL, " ");
    }

    // Normaliser les fréquences
    for (int i = 0; i < term_count; i++) {
        terms[i].tf /= word_count;
    }

    ProcessedDoc doc = {malloc(term_count * sizeof(Term)), term_count};
    memcpy(doc.terms, terms, term_count * sizeof(Term));

    free(copy);
    return doc;
}

// Fonction 3 : Transformation du corpus
ProcessedDoc* transform(char corpus[][1000], char* skills[], int skill_count) {
    ProcessedDoc* tcorpus = malloc(skill_count * sizeof(ProcessedDoc));

    for (int i = 0; i < skill_count; i++) {
        tcorpus[i] = preprocess(corpus[i], NULL, 0);
    }

    return tcorpus;
}

// Fonction 4 : Obtenir le vocabulaire
int get_vocab(ProcessedDoc* tcorpus, int skill_count, char vocab[][25]) {
    int vocab_size = 0;

    for (int i = 0; i < skill_count; i++) {
        for (int j = 0; j < tcorpus[i].size; j++) {
            bool found = false;
            for (int k = 0; k < vocab_size; k++) {
                if (strcmp(vocab[k], tcorpus[i].terms[j].term) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                strcpy(vocab[vocab_size], tcorpus[i].terms[j].term);
                vocab_size++;
            }
        }
    }

    return vocab_size;
}

// Fonction 5 : Obtenir les compétences
void get_skills(char* skills[], int skill_count, char result[][25]) {
    for (int i = 0; i < skill_count; i++) {
        strcpy(result[i], skills[i]);
    }
}

// Fonction 6 : Calculer TF vectoriel
void get_tf_vect(char vocab[][25], int vocab_size, ProcessedDoc doc, double* tf_vect) {
    for (int i = 0; i < vocab_size; i++) {
        tf_vect[i] = 0;
        for (int j = 0; j < doc.size; j++) {
            if (strcmp(vocab[i], doc.terms[j].term) == 0) {
                tf_vect[i] = doc.terms[j].tf;
                break;
            }
        }
    }
}

// Fonction 7 : Calculer la matrice TF
void get_tf_mat(char vocab[][25], int vocab_size, ProcessedDoc* tcorpus, int skill_count, double** tf_mat) {
    for (int i = 0; i < skill_count; i++) {
        get_tf_vect(vocab, vocab_size, tcorpus[i], tf_mat[i]);
    }
}

// Fonction 8 : Calculer le vecteur IDF
void get_idf_vect(double** tf_mat, int skill_count, int vocab_size, double* idf_vect) {
    for (int j = 0; j < vocab_size; j++) {
        int df = 0;
        for (int i = 0; i < skill_count; i++) {
            if (tf_mat[i][j] > 0) {
                df++;
            }
        }
        idf_vect[j] = log((double)skill_count / (df + 1));
    }
}

// Fonction 9 : Similitude cosinus
double cosine_sim(const double* x, const double* y, int size) {
    double dot = 0, norm_x = 0, norm_y = 0;

    for (int i = 0; i < size; i++) {
        dot += x[i] * y[i];
        norm_x += x[i] * x[i];
        norm_y += y[i] * y[i];
    }

    return dot / (sqrt(norm_x) * sqrt(norm_y));
}

// Fonction 10 : Trouver les compétences
void find_skills(const char* question, char vocab[][25], int vocab_size, char* skills[], int skill_count,
                 double** tf_idf_mat, double* idf_vect, double* sim) {
    ProcessedDoc qdoc = preprocess(question, vocab, vocab_size);
    double* qtf_idf = malloc(vocab_size * sizeof(double));
    get_tf_vect(vocab, vocab_size, qdoc, qtf_idf);

    for (int i = 0; i < vocab_size; i++) {
        qtf_idf[i] *= idf_vect[i];
    }

    for (int i = 0; i < skill_count; i++) {
        sim[i] = cosine_sim(tf_idf_mat[i], qtf_idf, vocab_size);
    }

    free(qtf_idf);
}