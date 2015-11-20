int read_multiple_words(char* line, int* position, char* out);
int read_int(char* line, int* position, int* out);
void handle_line(char* line, semesterData *data);
int read_config(char* fileName, semesterData *data);