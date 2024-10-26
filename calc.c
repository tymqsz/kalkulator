#define DEFAULT_CAPACITY 4
#define BASE 1000000000

#include "bignum.h"
#include<stdlib.h>
#include<stdio.h>


BigNum_t* add(BigNum_t* a, BigNum_t* b){
	BigNum_t* a_copy = copy_BigNum(a);
	BigNum_t* b_copy = copy_BigNum(b);

	int carry = 0;
	BigNum_t* result = init_BigNum(DEFAULT_CAPACITY);
	

	int max_ab_size = a->size > b->size ? a->size : b->size;
	add_leading_zeros(a_copy, max_ab_size);
	add_leading_zeros(b_copy, max_ab_size);
	
	long long single;
	int i = 0;
	while(carry > 0 || i < max_ab_size){
		if(i >= a_copy->size)
			single = carry;
		else
			single = carry + a_copy->digits[i] + b_copy->digits[i];

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
			expand_BigNum(result, result->capacity+1);

		
		i++;
	}

	destroy_BigNum(a_copy);
	destroy_BigNum(b_copy);

	
	return result;
}

BigNum_t* multiply(BigNum_t* a, BigNum_t* b){
	BigNum_t* a_copy = copy_BigNum(a);
	BigNum_t* b_copy = copy_BigNum(b);
	
	BigNum_t* result = int_to_BigNum(0);
	BigNum_t* current;
	BigNum_t* temp;
	
	long long single;
	int i = 0, j, carry;
	while(i < b_copy->size){
		j = 0;
		carry = 0;
		current = init_BigNum(a_copy->size+b_copy->size);
		
		while(j < a_copy->size || carry > 0){
			if(j >= a_copy->size)
				single = carry;
			else
				single = (long long)(b_copy->digits[i]) * a_copy->digits[j]+carry;
			
			if(single > (BASE-1)){
				current->digits[j] = (int)(single % BASE);
				carry = (int)(single / BASE);
			}
			else{
				current->digits[j] = (int)single;
				carry = 0;
			}

			if(++current->size >= current->capacity)
				expand_BigNum(current, current->capacity+2);
		
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
	
	destroy_BigNum(a_copy);
	destroy_BigNum(b_copy);

	return result;
}

int divide_by_digit(int a1, int a2, int b){
	long long a = (long long)a1*BASE + a2;
	long long div = a / b;	
	return (int)div;
}


BigNum_t* subtract(BigNum_t* a, BigNum_t* b){
	BigNum_t* a_copy = copy_BigNum(a);
	BigNum_t* b_copy = copy_BigNum(b);
	
	BigNum_t* result = init_BigNum(a->size);
	add_leading_zeros(b_copy, a->size);

	int i = 0;
	long long a_part;
	while(i < a_copy->size){
		a_part = a_copy->digits[i];
		if(b_copy->digits[i] > a_copy->digits[i]){
			// borrow
			a_part += BASE; 
			a_copy->digits[i+1] -= 1;
		}

		result->digits[result->size++] = (int)a_part - b_copy->digits[i];

		i += 1;
	}
	
	destroy_BigNum(a_copy);
	destroy_BigNum(b_copy);
	return result;
}

BigNum_t* single_divide(BigNum_t* a, int b){
	BigNum_t* a_copy = copy_BigNum(a);
	
	BigNum_t* result = init_BigNum(a->size);
	result->size = a->size;
	BigNum_t* big_b = int_to_BigNum(b);
	BigNum_t* partial;
	BigNum_t* b_shifted;
	BigNum_t* temp;

	int estimate, shift_cnt, j;
	add_leading_zeros(a_copy, a->size+1);
	int i = a_copy->size-1;
	while(i > 0){
		estimate = divide_by_digit(a_copy->digits[i], a_copy->digits[i-1], b);
		
		b_shifted = copy_BigNum(big_b);
		temp = int_to_BigNum(estimate);

		partial = multiply(big_b, temp);
		shift_cnt = i-1;
		j = 0;
		while(j < shift_cnt){
			shift_left(partial);
			shift_left(b_shifted);

			j++;
		}
		
		destroy_BigNum(temp);
		temp = subtract(a_copy, partial);
		
		destroy_BigNum(a_copy);
		a_copy = copy_BigNum(temp);
		destroy_BigNum(temp);
		destroy_BigNum(partial);
		destroy_BigNum(b_shifted);

		result->digits[i-1] = estimate;
		
		i--;
	}
	
	destroy_BigNum(a_copy);
	destroy_BigNum(big_b);
	
	return result;
}


BigNum_t* divide(BigNum_t* a, BigNum_t* b) {
    if (a->size == 1 && b->size == 1)
        return int_to_BigNum(a->digits[0] / b->digits[0]);
    if (a->size < b->size)
        return int_to_BigNum(0);
    if (b->size == 1)
        return single_divide(a, b->digits[0]);
	
	BigNum_t* temp;
	BigNum_t* one = int_to_BigNum(1);
    BigNum_t* result = init_BigNum(a->size);
    BigNum_t* quotient;
	int q; // ???
	BigNum_t* estimate;
	BigNum_t* h; //???

	add_leading_zeros(a, a->size + 1);
	BigNum_t* part = copy_BigNum(a);
	int i = 0;
	while(i < (a->size-b->size-1)){
		shift_right(part);
		i++;
	}
	

	i = a->size-1-b->size;
	result->size = i+1;
	while(i >= 0){
		quotient = single_divide(part, b->digits[b->size-1]);
		q = quotient->digits[b->size-1];
		
		h = int_to_BigNum(q);
		estimate = multiply(b, h);
		destroy_BigNum(h);

		while(compare(estimate, part)==1){
			q--;
			
			temp = subtract(estimate, b);
			destroy_BigNum(estimate);
			estimate = copy_BigNum(temp);
			destroy_BigNum(temp);
		
		}

		temp = subtract(part, estimate);
		destroy_BigNum(part);
		part = copy_BigNum(temp);
		destroy_BigNum(temp);
		
		// update part
		shift_left(part);
		part->size--;
		
		if(i > 0)
			part->digits[0] = a->digits[i-1];

		result->digits[i] = q; 
		

		destroy_BigNum(quotient);
		destroy_BigNum(estimate);
		i--;
		
	}
	
	destroy_BigNum(part);
	destroy_BigNum(one);

	return result;
}

BigNum_t* fast_exp(BigNum_t* base, BigNum_t* exponent){
	int n = exponent->digits[0]; // TODO: fix
	// declare y = 1
	BigNum_t* y = int_to_BigNum(1);
	// x as a copy of base
	BigNum_t* x = copy_BigNum(base);
	
	BigNum_t* temp;

	while(n > 1){
		if(n % 2 == 1){
			temp = multiply(x, y);
			destroy_BigNum(y);
			y = copy_BigNum(temp);
			destroy_BigNum(temp);

			n -= 1;
		}

		temp = multiply(x, x);
		destroy_BigNum(x);
		x = copy_BigNum(temp); 
		destroy_BigNum(temp);

		n /= 2;
	}

	BigNum_t* result = multiply(x, y);
	destroy_BigNum(y);
	destroy_BigNum(x);

	return result;
}

BigNum_t* exponentiate(BigNum_t* base, BigNum_t* exponent){
	return fast_exp(base, exponent);
}
