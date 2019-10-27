#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <iostream>
#include <map>
#include <string>

#include "filewords.h"

using namespace std;

map<string, int> words_occurences;

const char* yes_response = "YES ";
const char* no_response = "NO";
bool is_file_open = false;
bool is_file_closed = false;
bool is_populated = false;

const char* filename = "file.txt";
FILE* file;

bool isFileOpen() {
    return is_file_open;
}

bool isFileClosed() {
    return is_file_closed;
}

void openFileSearch() {
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    is_file_open = true;
    is_file_closed = false;
}

void openFileAppend() {
    file = fopen(filename, "w+");
    is_file_open = true;
    is_file_closed = false;
}

void closeFile() {
    int rd = fclose(file);
    if (rd < 0) {
        perror("File cannot be closed\n");
        exit(EXIT_FAILURE);
    }
}

bool is_word_in_map(char* word) {
    return words_occurences.find(string(word)) != words_occurences.end();
}

char* clean_word (char* word) {
    if (word[strlen(word) - 1] == ',' 
        || word[strlen(word) - 1] == '.'
        || word[strlen(word) - 1] == ':'
        || word[strlen(word) - 1] == '?'
        || word[strlen(word) - 1] == '!'
        || word[strlen(word) - 1] == '('
        || word[strlen(word) - 1] == ')'
        || word[strlen(word) - 1] == '-'
        || word[strlen(word) - 1] == ';') {
        word[strlen(word) - 1] = '\0';
    }

    if (word[0] == ',' 
        || word[0] == '.'
        || word[0] == ':'
        || word[0] == '?'
        || word[0] == '!'
        || word[0] == '('
        || word[0] == ')'
        || word[0] == '-'
        || word[0] == ';') {
        for (int i = 0; i < strlen(word) - 1; ++i) {
            word[i] = word[i + 1];
        }
        word[strlen(word) - 1] = '\0';
    }

    return word;
}

void populateMap() {
    openFileSearch();
    char* word = (char*) malloc (100 * sizeof(char));
    while (fscanf(file, "%s", word) != EOF) {
        word = clean_word(word);
        if (is_word_in_map(word)) {
            words_occurences[string(word)] = words_occurences[string(word)] + 1;
        } else {
            words_occurences[string(word)] = 1;
        }
    }
    free(word);
    closeFile();
    is_populated = true;
}

output1* get_server_response_1_svc(input1 *in, struct svc_req* cl) {
    if (!is_populated) {
        populateMap();
    }
    output1* result = (output1*) malloc (sizeof(output1));
    result->response = (char*) malloc (20 * sizeof(char));
    if (strcmp(in->command, "APPEND") == 0 || strcmp(in->command, "SEARCH") == 0) {
        result->response = strdup("YES ");
        strcat(result->response, in->command);
    } else {
        result->response = strdup("NO");
    }
	return result;
}

output2* get_server_word_and_occurences_1_svc(input2 *in, struct svc_req* cl) {
    output2* result = (output2*) malloc (sizeof(output2));
    result->found_word = strdup(in->word);
    result->occurences = 0;
    if (is_word_in_map(result->found_word)) {
        result->occurences = words_occurences[string(result->found_word)];
    }
    return result;
}

output3* get_server_all_word_same_size_1_svc(input3 *in, struct svc_req* cl) {
    output3* result = (output3*) malloc (sizeof(output3));
    result->no_words_same_size = 0;
    for (auto const& x : words_occurences) {
        if (x.first.length() == in->n_chars) {
            result->no_words_same_size++;
        }
    }
    return result;
}

output4* get_server_append_word_file_1_svc(input2 *in, struct svc_req* cl) {
    output4* result = (output4*) malloc (sizeof(output4));
    result->word_occurences = 0;
    file = fopen(filename, "a");
    
    if (file == NULL) {
        result->confirm_append_word = strdup("APPEND FAILED");
        return result;
    }

    int res = fprintf(file, " %s ", in->word);
    if (res < 0) {
        result->confirm_append_word = strdup("APPEND FAILED");
        return result;
    }

    int rd = fclose(file);
    if (rd < 0) {
        result->confirm_append_word = strdup("APPEND FAILED");
        return result;
    }

    if (is_word_in_map(in->word)) {
        words_occurences[string(in->word)] = words_occurences[string(in->word)] + 1;
    } else {
        words_occurences[string(in->word)] = 1;
    }

    result->word_occurences = words_occurences[string(in->word)];
    result->confirm_append_word = strdup("APPEND SUCCEDED");

    return result;
}