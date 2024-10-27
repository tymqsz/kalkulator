#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "ctype.h"
#include "bignum.h"
#include "file_io.h"
#include "calc.h"

#define MAX_LINE 1000000
#define MAX_ARGS 100

#define OUT "c_out.txt"

char* calculate(int arg_cnt, BigNum_t* args[], char oper, int base){
	if(arg_cnt != 2)
		exit(1);

	switch (oper) {
        case '+':
            add(&args[0], args[1]);
            break;
        case '*':
            multiply(&args[0], args[1]);
            break;
        case '/':  // Use '/' instead of '\\' for division
            divide(&args[0], args[1]);
            break;
        case '^':
            exponentiate(&args[0], args[1]);
            break;
        default:
            fprintf(stderr, "Error: Unsupported operation '%c'.\n", oper);
            return NULL;
    }
	
	char* result = BigNum_to_string(args[0]);
	return result;
}

char* change_base(int arg_cnt, BigNum_t* args[], int base1, int base2){
	printf("changing base\n");

	return NULL;
}

char* strip(char *str) {
	while (isspace((unsigned char)*str)) str++;
	if (*str == '\0') return str;
	char *end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;
	*(end + 1) = '\0';
	return str;
}

void process_input_file(char* filename){
	FILE* file = fopen(filename, "r");
	FILE* out = fopen(OUT, "w");
	int empty_cnt;
	char* line =  malloc(MAX_LINE);
	int base, target_base;
	char op;
	BigNum_t* args[MAX_ARGS];
	char* result = malloc(MAX_LINE);
	int arg_cnt = 0;
	int base_change;

	while(fgets(line, MAX_LINE, file)){
		base_change = 1;
		if(sscanf(line, "%d %d", &base, &target_base)!=2){
			sscanf(line, "%c %d", &op, &base);
			base_change = 0;
		}

		empty_cnt = 0;
		arg_cnt = 0;
		while(empty_cnt < 3 && fgets(line, MAX_LINE, file)){
			if(line[0] == '\n')
				empty_cnt++;
			else{
				empty_cnt = 0;
				
				line = strip(line);
				args[arg_cnt++] = string_to_BigNum(line);
			}
		}
		
		if(base_change)
			result = change_base(arg_cnt, args, base, target_base);
		else
			result = calculate(arg_cnt, args, op, base);
		
		fprintf(out, "%s\n\n", result);
		//for(int i = 0; i < arg_cnt; i++)
		//	destroy_BigNum(args[i]);
	}

}



int main(int argc, char** argv){
	char filename[] = "input.txt";
	
	process_input_file(filename);

	return 0;

}

