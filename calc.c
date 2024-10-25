#define DEFAULT_CAPACITY 4
#define BASE 1000000000

#include "bignum.h"
#include<stdlib.h>
#include<stdio.h>


BigNum_t* add(BigNum_t* a, BigNum_t* b){
	int carry = 0;
	BigNum_t* result = init_BigNum(DEFAULT_CAPACITY);
	

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
			expand_BigNum(result, result->capacity+1);

		
		i++;
	}
	
	return result;
}

BigNum_t* multiply(BigNum_t* a, BigNum_t* b){
	BigNum_t* result = int_to_BigNum(0);
	BigNum_t* current;
	BigNum_t* temp;
	
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

	return result;
}

int divide_by_digit(int a1, int a2, int b){
	long long a = (long long)a1*BASE + a2;
	long long div = a / b;	
	return (int)div;
}


BigNum_t* subtract(BigNum_t* a, BigNum_t* b){
	BigNum_t* result = init_BigNum(a->size);
	add_leading_zeros(b, a->size);

	int i = 0;
	long long a_part;
	while(i < a->size){
		a_part = a->digits[i];
		if(b->digits[i] > a->digits[i]){
			// borrow
			a_part += BASE; 
			a->digits[i+1] -= 1;
		}

		result->digits[result->size++] = (int)a_part - b->digits[i];

		i += 1;
	}

	return result;
}

BigNum_t* single_divide(BigNum_t* a, int b){
	printf("singling\n");
	BigNum_t* result = init_BigNum(a->size);
	result->size = a->size;
	BigNum_t* big_b = int_to_BigNum(b);
	BigNum_t* partial;

	add_leading_zeros(a, a->size+1);
	int i = a->size-1;
	while(i > 0){
		int estimate = divide_by_digit(a->digits[i], a->digits[i-1], b);
		
		BigNum_t* b_shifted = copy_BigNum(big_b);
		partial = multiply(big_b, int_to_BigNum(estimate));
		int shift_cnt = i-1;
		int j = 0;
		while(j < shift_cnt){
			shift_left(partial);
			shift_left(b_shifted);

			j++;
		}
		
		BigNum_t* temp;
		while(compare(partial, a) == 1){
			estimate -= 1;
			temp = subtract(partial, b_shifted);
			partial = copy_BigNum(temp);
			destroy_BigNum(temp);
		}
		temp = subtract(a, partial);
		
		printf("estimate: %d\n\n", estimate);
		print_BigNum(a);
		printf("-\n");
		print_BigNum(partial);
		printf("=\n");
		a = copy_BigNum(temp);
		print_BigNum(a);
		printf("---------------------------\n\n");
		result->digits[i-1] = estimate;
		
		i--;
	}

	return result;
}




BigNum_t* divide(BigNum_t* a, BigNum_t* b){
	if(a->size == 1 && b->size == 1)
		return int_to_BigNum(a->digits[0] / b->digits[0]);
	if(a->size < b->size)
		return int_to_BigNum(0);
	
	if(b->size == 1)
		return single_divide(a, b->digits[0]);

	BigNum_t* result = init_BigNum(1000000);
	result->size = a->size;
	BigNum_t* partial;
	int estimate;
	int i = a->size-1;
	while(i >= b->size-1){
		if(b->size == 1)
			estimate = divide_by_digit(0, a->digits[i], b->digits[b->size-1]);
		else
			estimate = divide_by_digit(a->digits[i], a->digits[i-1], b->digits[b->size-1]);
		
		BigNum_t* b_shifted = copy_BigNum(b);
		partial = multiply(b, int_to_BigNum(estimate));
		int shift_cnt = (i-b->size+1);
		int j = 0;
		while(j < shift_cnt){
			shift_left(partial);
			shift_left(b_shifted);

			j++;
		}
		
		print_BigNum(partial);
		while(compare(partial, a) == 1){
			estimate -= 1;
			BigNum_t* temp = subtract(partial, b_shifted);
		}
		printf("est: %d\n", estimate);	
		result->digits[i] = estimate;

		i--;
	}

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
