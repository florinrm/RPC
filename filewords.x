struct input1 {
    string command<>;
};

struct output1 {
    string response<>;
};

struct input2 {
    string word<>;
};

struct output2 {
    string found_word<>;
    int occurences;
};

struct input3 {
    int n_chars;
};

struct output3 {
    int no_words_same_size;
};

struct output4 {
    string confirm_append_word<>;
    int word_occurences;
};

program CHECKPROG {
	version CHECKVERS {
			output1 GET_SERVER_RESPONSE(input1) = 1;
            output2 GET_SERVER_WORD_AND_OCCURENCES(input2) = 2;
            output3 GET_SERVER_ALL_WORD_SAME_SIZE(input3) = 3;
            output4 GET_SERVER_APPEND_WORD_FILE(input2) = 4;
	} = 1;
} = 123456789;