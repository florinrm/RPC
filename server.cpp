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
    is_file_open = true;
    is_file_closed = false;
}

void openFileAppend() {
    file = fopen(filename, "w+");
    is_file_open = true;
    is_file_closed = false;
}

void closeFile() {
    fclose(file);
    is_file_open = false;
    is_file_closed = true;
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
    char* word;
    while (fscanf(file, "%s", word) != EOF) {
        word = clean_word(word);
        if (is_word_in_map(word)) {
            words_occurences[string(word)] = words_occurences[string(word)] + 1;
        } else {
            words_occurences[string(word)] = 1;
        }
    }
    closeFile();
    is_populated = true;

    /*
    for (auto const& x : words_occurences) {
        std::cout << x.first  // string (key)
                << ':' 
                << x.second // string's value 
                << std::endl;
    } */
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
    return result;
}

output4* get_server_append_word_file_1_svc(input2 *in, struct svc_req* cl) {
    output4* result = (output4*) malloc (sizeof(output4));
    return result;
}