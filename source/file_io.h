#ifndef FIO
#define FIO

char* calculate(int arg_cnt, char* args[], char oper, int base);

char* change_base(int arg_cnt, char* args[], int base1, int base2);

char* strip(char *str);

void process_input_file(char* filename, char*);
#endif
