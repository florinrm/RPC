#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <rpc/rpc.h>

#include "filewords.h"

#define RMACHINE "localhost"

int main (int argc, char** argv) {

    CLIENT* cl = clnt_create(RMACHINE, CHECKPROG, CHECKVERS, "tcp");

    if (cl == NULL) {
		perror("Client not created.\n The process has been killed with the following status");
		return -1;
	}

    while(true) {

        printf("Write operation:\n");
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

            fgets(command, 20, stdin);
            if (command[strlen(command) - 1] == '\n') {
                command[strlen(command) - 1] = '\0';
            }
            client_request->command = strdup(command);
            server_response = get_server_response_1(client_request, cl);
            printf("Server response: %s\n", server_response->response);

        }

        if (strcmp(client_request->command, "SEARCH") == 0) {

            printf("Search a word or words with a given number of characters?\n");
            printf("Press 0 for word?\n");
            printf("Press 1 for words with same number of characters?\n");
            
            int n;
            scanf("%d", &n);
            while (n > 1 || n < 0) {
                printf("Press 0 for word?\n");
                printf("Press 1 for words with same number of characters?\n");
                scanf("%d", &n);
            }

            if (n == 0) {

                printf("Write the word to be searched in file\n");
                char* word = (char*) malloc (100 * sizeof(char));
                scanf("%s", word);

                input2* to_search = (input2*) malloc (sizeof(input2));
                to_search->word = strdup(word);

                output2* word_occurences = get_server_word_and_occurences_1(to_search, cl);

                printf("%s appears %d times in file\n", word_occurences->found_word, word_occurences->occurences);

            } else if (n == 1) {

                printf("Write the number of characters to search in file\n");
                input3* number_of_chars = (input3*) malloc (sizeof(input3));
                scanf("%d", &number_of_chars->n_chars);
                
                output3* no_words = get_server_all_word_same_size_1(number_of_chars, cl);

                printf("There are %d words with same length in file\n", no_words->no_words_same_size);
            }

        } else if (strcmp(client_request->command, "APPEND") == 0) {

            printf("Write the word to append in file\n");
            char* word = (char*) malloc (100 * sizeof(char));
            scanf("%s", word);

            input2* to_append = (input2*) malloc (sizeof(input2));
            to_append->word = strdup(word);

            output4* appended = get_server_append_word_file_1(to_append, cl);
            if (strcmp(appended->confirm_append_word, "NO") == 0) {
                printf("Word %s wasn't appended in file\n", word);
            } else if (strcmp(appended->confirm_append_word, "YES") == 0) {
                printf("Word %s was appended successfully in file, appearing \n", word);
            }
        }
        // trick
        int c;
        do {
            c = getchar();
        } while(c != EOF && c != '\n');
    }

    return 0;
}