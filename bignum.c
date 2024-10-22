#include "bignum.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


BigNum_t* init_BigNum(int capacity){
	BigNum_t* num = malloc(sizeof(*num));
	
	num->size = 0;
	num->capacity = capacity;
	num->digits = calloc(capacity, sizeof(int));
	
	return num;
}

void destroy_BigNum(BigNum_t* num){
	free(num->digits);
	free(num);
}

void expand_BigNum(BigNum_t* num, int capacity){
	num->capacity = capacity;
	
	num->digits = realloc(num->digits, sizeof(int)*capacity);

	for(int i = num->size; i < num->capacity; i++)
		num->digits[i] = 0;
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
	while(num->size < target_size){
		if(num->size+1 >= num->capacity)
			expand_BigNum(num, num->capacity*2);
		
		num->digits[num->size++] = 0;
	}
}

BigNum_t* copy_BigNum(BigNum_t* num){
	BigNum_t* copy = init_BigNum(num->capacity);
	copy->size = num->size;

	int i = 0;
	while(i < num->size){
		copy->digits[i] = num->digits[i];
		i++;
	}
	
	return copy;
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
