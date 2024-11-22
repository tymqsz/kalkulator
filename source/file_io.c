#include<stdio.h>
#include "bignum.h"
#include<stdlib.h>
#include<string.h>
#include "calc.h"
#include<ctype.h>

#define MAX_ARG_LEN 1000
#define MAX_ARG_CNT 100

char* calculate(int arg_cnt, char* args[], char oper, int base){
	
	BigNum_t* modulo = init_BigNum(4);
	BigNum_t* a;
	BigNum_t* b;
	if(base == 10)
		a = string_to_BigNum(args[0]);
	else
		a = convert_to_decimal(args[0], base);

	int i = 1;
	while(i < arg_cnt){
		if(base == 10){
			b = string_to_BigNum(args[i]);
		}
		else{
			b = convert_to_decimal(args[i], base);
		}

		switch (oper) {
			case '+':
				add(&a, b);
				break;
			case '*':
				multiply(&a, b);
				break;
			case '/':  
				divide(&a, b, &modulo);
				break;
			case '%':
				divide(&a, b, &modulo);
				destroy_BigNum(a);
				a = copy_BigNum(modulo);
				break;
			case '^':
				exponentiate(&a, b);
				break;
			default:
				fprintf(stderr, "Error: Unsupported operation '%c'.\n", oper);
				return NULL;
		}

		i++;
	}
	
	char* result;
	if(base == 10)
		result = BigNum_to_string(a);
	else
		result = convert_from_decimal(a, base);
	
	destroy_BigNum(a);
	destroy_BigNum(b);
	destroy_BigNum(modulo);
	
	return result;
}

char* change_base(int arg_cnt, char* args[], int base1, int base2){
	if(arg_cnt != 1)
		return "\0";
	
	BigNum_t* decimal = convert_to_decimal(args[0], base1);
	char* result = convert_from_decimal(decimal, base2);
	
	destroy_BigNum(decimal);
	return result; 
}

char* strip(char *str) {
	while (isspace((unsigned char)*str)) str++;
	if (*str == '\0') return str;
	char *end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;
	*(end + 1) = '\0';
	return str;
}


int is_oper_line(char* line){
	char oper;
	int base1, base2;
	
    if (sscanf(line, " %c %d", &oper, &base1) == 2) {
		if(strchr(line, ' '))
			return 1;
    }

    if (sscanf(line, " %d %d", &base1, &base2) == 2) {
        if(strchr(line, ' '))
			return 1;
    }
	
	return 0;
}


void process_input_file(char* in, char* out){
    FILE* in_file = fopen(in, "r");
	FILE* out_file = fopen(out, "w");

    int arg_cnt = 0;
    int base1, base2;
    char oper;
    char** args = malloc(sizeof(*args) * MAX_ARG_CNT);
    int i;
    for (i = 0; i < MAX_ARG_CNT; i++) {
        args[i] = malloc(MAX_ARG_LEN);
    }
	
	char* output;
    char line[MAX_ARG_LEN];
    int empty_line_cnt = 0;
    int base_conversion = 0; // Default to 0; adjust as needed

    while (fgets(line, MAX_ARG_LEN, in_file)) {
        // Remove trailing newline
        line[strcspn(line, "\r\n")] = '\0';

		if(line[0] == '\0'){
			empty_line_cnt++;
			if (empty_line_cnt == 3) {
				if (base_conversion)
					output = change_base(arg_cnt, args, base1, base2);
				else
					output = calculate(arg_cnt, args, oper, base1);
				
				fprintf(out_file, "%s\n\n", output);
				arg_cnt = 0;

				// Free args
				for (i = 0; i < MAX_ARG_CNT; i++) {
					free(args[i]);
					args[i] = malloc(MAX_ARG_LEN);
				}

			}
		}
		else{
			empty_line_cnt = 0;
			if (is_oper_line(line)==1) {
				if (sscanf(line, "%d %d", &base1, &base2) == 2) {
                base_conversion = 1;
				} else if (sscanf(line, "%c %d", &oper, &base1) == 2) {
					base_conversion = 0;
				}
				fprintf(out_file, "%s\n\n", line);
			} else {
				strcpy(args[arg_cnt++], line);
				fprintf(out_file, "%s\n\n", line);
			}
		}
	}

	if (base_conversion)
		output = change_base(arg_cnt, args, base1, base2);
	else
		output = calculate(arg_cnt, args, oper, base1);	
	fprintf(out_file, "%s\n", output);


	fclose(in_file);
    fclose(out_file);

    // Free memory
    for (i = 0; i < MAX_ARG_CNT; i++) {
        free(args[i]);
    }
    free(args);
}
