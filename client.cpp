#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <rpc/rpc.h>

#include "filewords.h"

#define RMACHINE "localhost"

bool isValidNumber(char * str) {
   for (int i = 0; i < strlen(str); ++i) {
        if (str[i] < 48 || str[i] > 57) {
            return false;
        }
    }
    return true;
}

int main (int argc, char** argv) {

    CLIENT* cl = clnt_create(RMACHINE, CHECKPROG, CHECKVERS, "tcp");

    if (cl == NULL) {
		perror("Client not created.\n The process has been killed with the following status");
		return -1;
	}

    while(true) {

        printf("Write operation [SEARCH, APPEND, exit]:\n");
        char* command = (char*) malloc (20 * sizeof(char));
        fgets(command, 20, stdin);
        if (command[strlen(command) - 1] == '\n') {
            command[strlen(command) - 1] = '\0';
        }

        if (strcmp(command, "exit") == 0) {
            printf("Now exiting...\n");
            break;
        }
        
        input1* client_request = (input1*) malloc (sizeof(input1));
        client_request->command = strdup(command);

        output1* server_response = get_server_response_1(client_request, cl);
        printf("Server response: %s\n", server_response->response);

        while (strncmp(server_response->response, "NO", 2) == 0) {
            printf("Write operation [SEARCH, APPEND, exit]:\n");
            fgets(command, 20, stdin);
            if (command[strlen(command) - 1] == '\n') {
                command[strlen(command) - 1] = '\0';
            }
            client_request->command = strdup(command);
            server_response = get_server_response_1(client_request, cl);
            printf("Server response: %s\n", server_response->response);

        }

        if (strcmp(client_request->command, "SEARCH") == 0) {

            printf("Search a word or words with a given number of characters\n");
            
            char* word = (char*) malloc (100 * sizeof(char));
            fgets(word, 100, stdin);

            if (word[strlen(word) - 1] == '\n') {
                word[strlen(word) - 1] = '\0';
            }

            if (!isValidNumber(word)) {

                printf("Searching the word in file...\n");

                input2* to_search = (input2*) malloc (sizeof(input2));
                to_search->word = strdup(word);

                output2* word_occurences = get_server_word_and_occurences_1(to_search, cl);

                printf("%s appears %d times in file\n",
                    word_occurences->found_word, word_occurences->occurences);

            } else {

                input3* number_of_chars = (input3*) malloc (sizeof(input3));
                number_of_chars->n_chars = atoi(word);
                printf("Searching the word with the length of %d in file...\n", number_of_chars->n_chars);
                
                output3* no_words = get_server_all_word_same_size_1(number_of_chars, cl);

                printf("There are %d words with length of %d in file\n",
                    no_words->no_words_same_size, number_of_chars->n_chars);
            }

        } else if (strcmp(client_request->command, "APPEND") == 0) {

            printf("Write the word to append in file\n");
            char* word = (char*) malloc (100 * sizeof(char));
            fgets(word, 100, stdin);

            if (word[strlen(word) - 1] == '\n') {
                word[strlen(word) - 1] = '\0';
            }

            input2* to_append = (input2*) malloc (sizeof(input2));
            to_append->word = strdup(word);

            output4* appended = get_server_append_word_file_1(to_append, cl);
            if (strcmp(appended->confirm_append_word, "APPEND FAILED") == 0) {
                printf("Word %s wasn't appended in file\n", word);
            } else if (strcmp(appended->confirm_append_word, "APPEND SUCCEDED") == 0) {
                printf("Word %s was successfully appended in file, appearing %d times\n",
                    word, appended->word_occurences);
            }
        }
    }

    return 0;
}