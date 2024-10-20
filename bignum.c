#include "bignum.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


BigNum_t* init_BigNum(int capacity){
	BigNum_t* num = malloc(sizeof(*num));
	
	num->size = 0;
	num->capacity = capacity;
	num->digits = malloc(sizeof(int)*capacity);
	
	return num;
}

void expand_BigNum(BigNum_t* num, int capacity){
	num->capacity = capacity;
	
	num->digits = realloc(num->digits, sizeof(int)*capacity);
}

void shift_left(BigNum_t* num){
	num->size++;
	if(num->size >= num->capacity)
		expand_BigNum(num, num->capacity+1);
	
	int i = num->size-1;
	while(i > 0){
		num->digits[i] = num->digits[i-1];
		i--;
	}

	num->digits[0] = 0;
}

void add_leading_zeros(BigNum_t* num, int target_size){
	while(num->size < target_size)
		num->digits[num->size++] = 0;
}

BigNum_t* string_to_BigNum(char* string){
	BigNum_t* num = init_BigNum(3);	

	int len = strlen(string);
	int start, size;
	char* digit;
	int i = len;

	while(i > 0){
		start = (i-9) >= 0 ? (i-9) : 0;
		size = (i-9) >= 0 ? 9 : i;
		
		digit = malloc(size);
		strncpy(digit, string+start, size);
		
		if(num->size >= num->capacity)
			expand_BigNum(num, num->capacity*2);

		num->digits[num->size++] = atoi(digit);

		free(digit);
		i -= 9;
	}

	return num;
}


void print_BigNum(BigNum_t* num){
	int i = num->size-1;

	while(i >= 0){
		printf("%09d", num->digits[i]);
		i--;
	}

	printf("\n");
}













	

