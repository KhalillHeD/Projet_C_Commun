#include<stdio.h>
#include<ctype.h>
#include<strings.h>
#include <stdbool.h> 

bool is_alpha(const char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (!isalpha(word[i])) { 
            return false;
        }
    }
    return true;
}

int extract_unique_words(const char *text, const char exclude[][25], int exclude_size, char result[][25]) {
    char word[25] = "";
    int result_count = 0;
    int word_index = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        char c = tolower(text[i]);

        if (isspace(c) || ispunct(c)) {
            if (word_index > 0) { 
                word[word_index] = '\0';
                if (!is_in_vector(word, exclude, exclude_size) && !is_in_vector(word, result, result_count)) {
                    strcpy(result[result_count], word); 
                    result_count++;
                }
                word_index = 0;
            }
        } else {
            if (word_index < 24) {
                word[word_index++] = c;
            }
        }
    }

    if (word_index > 0) {
        word[word_index] = '\0';
        if (!is_in_vector(word, exclude, exclude_size) && !is_in_vector(word, result, result_count)) {
            strcpy(result[result_count], word);
            result_count++;
        }
    }

    return result_count;
}
bool is_in_vector(const char *word, const char vector[][25], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(word, vector[i]) == 0) {
            return true;
        }
    }
    return false;
}