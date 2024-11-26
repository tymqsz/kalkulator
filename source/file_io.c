#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "bignum.h"
#include "calc.h"
#include "file_io.h"

#define MAX_ARG_LEN 100000
#define MAX_ARG_CNT 100


char* calculate(int arg_cnt, char* args[], char operator, int base){
	BigNum_t* modulo = init_BigNum(4);
	BigNum_t* a;
	BigNum_t* b;
	
	/* load first argument */
	if(base == 10)
		a = string_to_BigNum(args[0]);
	else
		a = convert_to_decimal(args[0], base);
	
	/* do operations until no more arguments */
	int i = 1;
	while(i < arg_cnt){
		if(base == 10){
			b = string_to_BigNum(args[i]);
		}
		else{
			b = convert_to_decimal(args[i], base);
		}

		switch (operator) {
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
		}
		
		destroy_BigNum(b);
		i++;
	}
	
	/* convert result to a string,
	   change base if necessary */
	char* result;
	if(base == 10)
		result = BigNum_to_string(a);
	else
		result = convert_from_decimal(a, base);
	
	destroy_BigNum(a);
	destroy_BigNum(modulo);
	
	return result;
}

char* change_base(int arg_cnt, char* args[], int base1, int base2){
	if(arg_cnt != 1)
		return "\0";
	
	/* convert to decimal, from decimal to target base */
	BigNum_t* decimal = convert_to_decimal(args[0], base1);
	char* result = convert_from_decimal(decimal, base2);
	
	destroy_BigNum(decimal);
	return result; 
}

int is_oper_line(char* line){
	char operator;
	int base1, base2;
	
	/* check if line matches pattern of "operator base"
								     or "base1 base2" */
    if (sscanf(line, " %c %d", &operator, &base1) == 2) {
		if(strchr(line, ' '))
			return 1;
    }
    else if (sscanf(line, " %d %d", &base1, &base2) == 2) {
        if(strchr(line, ' '))
			return 1;
    }
	
	return 0;
}

int bases_ok(int base1, int base2){
	if(base1 < 2 || base1 > 16 || base2 < 2 || base2 > 16)
		return 0;
	
	return 1;
}

int operation_ok(char operator, int base){
	if(operator == 'f' || base == -1)
		return 0;

	char possible[] = "+/%^*";

	if(base < 2 || base > 16 || strchr(possible, operator) == NULL)
		return 0;
	
	return 1;
}

int argument_ok(char* arg, int base){
	if(base == -1)
		return 0;

	char all_digits[] = "0123456789ABCDEF";
	char accepted[16];
	strncpy(accepted, all_digits, base); /* copy digits from a given base */
	accepted[base] = '\0';
	
	/* check if each char of arg is a correct digit*/
	char* c = arg;
	while(*c != '\0'){
		if(strchr(accepted, *c) == NULL)
			return 0;

		c++;
	}

	return 1;
}


void process_input_file(char* in, char* out){
    FILE* in_file = fopen(in, "r");
	if(in_file == NULL){
		fprintf(stderr, "brak mozliwosci odczytu pliku wejsciowego\n");
		return;
	}
	FILE* out_file = fopen(out, "w");
	if(out_file == NULL){
		fprintf(stderr, "brak mozliwosci utworzenia pliku wyjsciowego\n");
		return;
	}

    int arg_cnt = 0;
    int base1 = -1, base2 = -1;
    char operator = 'f'; /* 'f' - meaning faulty */
    char** args = malloc(sizeof(*args) * MAX_ARG_CNT);
    int i;
    for (i = 0; i < MAX_ARG_CNT; i++) {
        args[i] = malloc(MAX_ARG_LEN);
    }
	
	char* output;
    char line[MAX_ARG_LEN];
    int empty_line_cnt = 0;
    int operation_type = -1; /* operation_type type: 0->arithmetics, 1->base change, -1->invalid */
	int arg_correct = 1; /* correctness of arguments: 1->ok, 0-> not ok */
	
    while (fgets(line, MAX_ARG_LEN, in_file) || (arg_cnt != 0 && operation_type != -1)) {
        
		/* removing whitespaces at the end */
        line[strcspn(line, "\r\n")] = '\0';

		if(line[0] == '\0'){
			empty_line_cnt++;

			/* if 3 empty lines in a row - perform operation_type */
			if (empty_line_cnt == 3) {
				
				/* check if there is enough args */
				if((operation_type == 1 && arg_cnt < 1) || (operation_type == 0 && arg_cnt < 2)){
					operation_type = -1;
				}

				if (operation_type == 1){
					if(arg_correct){
						output = change_base(arg_cnt, args, base1, base2);
					}
					else{
						output = malloc(MAX_ARG_LEN);
						strcpy(output, "argumenty niepoprawne");
					}
				}
				else if(operation_type == 0){
					if(arg_correct){
						output = calculate(arg_cnt, args, operator, base1);
					}
					else{
						output = malloc(MAX_ARG_LEN);
						strcpy(output, "argumenty niepoprawne");
					}
				}
				else{
					output = malloc(MAX_ARG_LEN);
					strcpy(output, "operacja niedozwolona");
				}
				
				fprintf(out_file, "%s\n\n", output);
				arg_cnt = 0;
				arg_correct = 1;
				free(output);
			}
		}
		else{
			empty_line_cnt = 0;
			
			if (is_oper_line(line)==1) {
				/* update current operation */	
				if (sscanf(line, "%d %d", &base1, &base2) == 2) {
					if(bases_ok(base1, base2) == 1){
						operation_type = 1;
					}
					else{
						base1 = -1;
						base2 = -1;
						operation_type = -1;
					}

				} else if (sscanf(line, "%c %d", &operator, &base1) == 2) {
					if(operation_ok(operator, base1) == 1){
						operation_type = 0;
					}
					else{
						operator = 'f';
						base1 = -1;
						operation_type = -1;
					}
				}
				fprintf(out_file, "%s\n\n", line);
			} else {
				/* load next argument */
				if(argument_ok(line, base1)){
					strcpy(args[arg_cnt], line);
				}
				else{
					arg_correct = 0; /* set incorrect arg flag */
				}
				fprintf(out_file, "%s\n\n", line);
				arg_cnt++;
			}
		}
	}
	
	fclose(in_file);
    fclose(out_file);

    for (i = 0; i < MAX_ARG_CNT; i++) {
        free(args[i]);
    }
    free(args);
}
