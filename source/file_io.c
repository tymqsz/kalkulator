#include<stdio.h>
#include "bignum.h"
#include<stdlib.h>

#define MAX_NUM_LEN 1000000000

void load_args_from_file(BigNum_t** a, BigNum_t** b, char* filename){
	FILE* file = fopen(filename, "r");
	if(file == NULL)
		return;
	
	char* string_a = malloc(MAX_NUM_LEN);
	char* string_b = malloc(MAX_NUM_LEN);
	
	fscanf(file, "%1000000s", string_a); 
	fscanf(file, "%1000000s", string_b); 
	
	*a = string_to_BigNum(string_a);
	*b = string_to_BigNum(string_b);
	
	free(string_a);
	free(string_b);
	
	fclose(file);
}

