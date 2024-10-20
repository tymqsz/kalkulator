#define DEFAULT_CAP 4
#define MAX_DIGIT 999999999

#include "bignum.h"
#include<stdlib.h>
#include<stdio.h>

BigNum_t* add(BigNum_t* a, BigNum_t* b){
	int carry = 0;
	BigNum_t* result = init_BigNum(DEFAULT_CAP);
	

	int max_ab_size = a->size > b->size ? a->size : b->size;
	add_leading_zeros(a, max_ab_size);
	add_leading_zeros(b, max_ab_size);
	
	long long single;
	int i = 0;
	while(carry > 0 || i < max_ab_size){
		if(i >= a->size)
			single = carry;
		else
			single = carry + a->digits[i] + b->digits[i];

		if(single > MAX_DIGIT){
			result->digits[i] = (int)(single - (MAX_DIGIT+1));
			
			carry = 1;
		}
		else{
			result->digits[i] = (int)single;
			
			carry = 0;
		}

		result->size++;
		if(result->size >= result->capacity)
			expand_BigNum(result, result->capacity*2);

		
		i++;
	}
	
	return result;
}

BigNum_t* multiply(BigNum_t* a, BigNum_t* b){
	BigNum_t* result = init_BigNum(b->size+a->size);
	BigNum_t* current = init_BigNum(b->size+a->size);
	BigNum_t* temp;
	
	// TODO: add zeroing bignums
	// TODO: add destroying bignums

	long long single;
	int i = 0, j, carry;
	while(i < b->size){
		j = 0;
		carry = 0;
		current = init_BigNum(a->size+b->size);

		while(j < a->size || carry > 0){
			if(j >= a->size)
				single = carry;
			else
				single = (long long)(b->digits[i]) * a->digits[j]+carry;
			
			if(single > MAX_DIGIT){
				current->digits[j] = (int)(single % 1000000000);
				carry = (int)(single / 1000000000);
			}
			else{
				current->digits[j] = (int)single;
				carry = 0;
			}

			if(++current->size >= current->capacity)
				expand_BigNum(current, current->capacity*2);

			j++;
		}
		
		for(int shift_cnt = 0; shift_cnt < i; shift_cnt++)
			shift_left(current);

		temp = add(result, current);
		free(result);
		result = temp;
		free(current);

		i++;
	}


	return result;
}
			

