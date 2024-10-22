#define DEFAULT_CAP 4
#define BASE 1000000000

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

		if(single > (BASE-1)){
			result->digits[i] = (int)(single - BASE);
			
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
			
			if(single > (BASE-1)){
				current->digits[j] = (int)(single % BASE);
				carry = (int)(single / BASE);
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
		
		destroy_BigNum(result);
		result = copy_BigNum(temp);
		
		destroy_BigNum(current);
		destroy_BigNum(temp);
		
		i++;
	}

	return result;
}
			
BigNum_t* exponentiate(BigNum_t* base, BigNum_t* exponent){
	BigNum_t* result = copy_BigNum(base);
	/* TODO: zero/ one bignums, compare
	if(base->size == 1 && base->digit[0] == 0)
		return one_BigNum();
	*/
	BigNum_t* temp;
	
	int i = 0;
	while(i < exponent->digits[0]-1){ // TODO fix
		temp = multiply(result, base);
		destroy_BigNum(result);
		result = copy_BigNum(temp);
		destroy_BigNum(temp);
		
		i++;
	}
	
	return result;
}

