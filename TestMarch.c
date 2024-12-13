#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_TERMS 100 // Maximum terms per document
#define MAX_DOCS 10   // Maximum documents in the corpus

// Function prototypes
float tf(char term[], char doc[][25], int doc_size);
float idf(char term[], int doc_count, char corpus[][MAX_TERMS][25]);
void vectorize_doc(char doc[][25], int doc_size, char vocab[][25], int vocab_size, float vector[], char corpus[][MAX_TERMS][25], int doc_count);
void cosine_similarity(float query_vec[], float doc_vec[], int size, float *similarity);

// Main function
int main() {
    // Example corpus with 3 documents
    char corpus[3][MAX_TERMS][25] = {
        {"this", "is", "a", "sample"},
        {"this", "sample", "is", "another", "example"},
        {"yet", "another", "example", "document"}
    };
    int doc_sizes[3] = {4, 5, 4}; // Sizes of each document in the corpus
    int doc_count = 3; // Total number of documents

    // Vocabulary (built manually or extracted programmatically)
    char vocab[10][25] = {"this", "is", "a", "sample", "another", "example", "yet", "document"};
    int vocab_size = 8; // Total number of unique terms

    // Step 3: Vectorization of corpus documents
    float tfidf_vectors[MAX_DOCS][MAX_TERMS] = {0};
    for (int i = 0; i < doc_count; i++) {
        vectorize_doc(corpus[i], doc_sizes[i], vocab, vocab_size, tfidf_vectors[i], corpus, doc_count);
    }

    // Step 4: Vectorization of query
    char query[] = "sample document";
    char query_terms[MAX_TERMS][25];
    int query_size = 0;
    char *token = strtok(query, " ");
    while (token != NULL) {
        strcpy(query_terms[query_size++], token);
        token = strtok(NULL, " ");
    }
    float query_vector[MAX_TERMS] = {0};
    vectorize_doc(query_terms, query_size, vocab, vocab_size, query_vector, corpus, doc_count);

    // Step 5: Similarity calculation
    printf("Similarities:\n");
    for (int i = 0; i < doc_count; i++) {
        float similarity = 0;
        cosine_similarity(query_vector, tfidf_vectors[i], vocab_size, &similarity);
        printf("Document %d similarity: %.2f\n", i + 1, similarity);
    }

    return 0;
}

// Step 1 & 2: Calculate TF
float tf(char term[], char doc[][25], int doc_size) {
    int count = 0;
    for (int i = 0; i < doc_size; i++) {
        if (strcmp(doc[i], term) == 0) {
            count++;
        }
    }
    return (float)count / doc_size;
}

// Step 2: Calculate IDF
float idf(char term[], int doc_count, char corpus[][MAX_TERMS][25]) {
    int docs_with_term = 0;
    for (int i = 0; i < doc_count; i++) {
        for (int j = 0; j < MAX_TERMS && strlen(corpus[i][j]) > 0; j++) {
            if (strcmp(corpus[i][j], term) == 0) {
                docs_with_term++;
                break;
            }
        }
    }
    if (docs_with_term == 0) return 0;
    return log((float)doc_count / docs_with_term);
}

// Step 3: Vectorize document
void vectorize_doc(char doc[][25], int doc_size, char vocab[][25], int vocab_size, float vector[], char corpus[][MAX_TERMS][25], int doc_count) {
    for (int i = 0; i < vocab_size; i++) {
        float term_tf = tf(vocab[i], doc, doc_size);  // Term Frequency
        float term_idf = idf(vocab[i], doc_count, corpus);  // Inverse Document Frequency
        vector[i] = term_tf * term_idf;  // TF-IDF
    }
}

// Step 5: Calculate Cosine Similarity
void cosine_similarity(float query_vec[], float doc_vec[], int size, float *similarity) {
    float dot_product = 0, query_norm = 0, doc_norm = 0;
    for (int i = 0; i < size; i++) {
        dot_product += query_vec[i] * doc_vec[i];
        query_norm += query_vec[i] * query_vec[i];
        doc_norm += doc_vec[i] * doc_vec[i];
    }
    *similarity = dot_product / (sqrt(query_norm) * sqrt(doc_norm));
}
