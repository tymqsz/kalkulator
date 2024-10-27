#define DEFAULT_CAPACITY 4
#define BASE 1000000000

#include "bignum.h"
#include<stdlib.h>
#include<stdio.h>


void add(BigNum_t** a, BigNum_t* b){
	BigNum_t* A = *a;
	BigNum_t* B = copy_BigNum(b);
	BigNum_t* result = init_BigNum(DEFAULT_CAPACITY);
	
	/* normalize numbers length */
	int max_ab_size = (*a)->size > b->size ? (*a)->size : b->size;
	add_leading_zeros(A, max_ab_size);
	add_leading_zeros(B, max_ab_size);
	

	long long digit;
	int carry = 0, i = 0;
	while(carry > 0 || i < max_ab_size){	
		if(i >= A->size) /* handle last carry */
			digit = carry;
		else
			digit = carry + A->digits[i] + B->digits[i];

		/* handle digit overflow */
		if(digit > (BASE-1)){
			result->digits[i] = (int)(digit - BASE);
			carry = 1;
		}
		else{
			result->digits[i] = (int)digit;
			carry = 0;
		}
		
		/* update result size, 
		   if necessary allocate more memory */
		if(++result->size >= result->capacity)
			expand_BigNum(result, result->capacity+1);

		i++;
	}

	destroy_BigNum(A);
	destroy_BigNum(B);
	
	*a = result;
}

void multiply(BigNum_t** a, BigNum_t* b){
	BigNum_t* A = (*a);
	BigNum_t* B = copy_BigNum(b);
	
	BigNum_t* result = int_to_BigNum(0);
	BigNum_t* part;
	
	/* long multiplication by adding up partial results (part) */
	long long digit;
	int i = 0, j, carry;
	while(i < B->size){
		j = 0;
		carry = 0;
		part = init_BigNum(A->size+B->size);
		
		while(j < A->size || carry > 0){
			if(j >= A->size) /* handle last carry */
				digit = carry;
			else
				digit = (long long)(B->digits[i]) * A->digits[j]+carry;
			
			/* handle digit overflow */
			if(digit > (BASE-1)){
				part->digits[j] = (int)(digit % BASE);
				carry = (int)(digit / BASE);
			}
			else{
				part->digits[j] = (int)digit;
				carry = 0;
			}
			
			/* update part size and if necessary allocate more memory */
			if(++part->size >= part->capacity)
				expand_BigNum(part, part->capacity+1);
		
			j++;
		}
		
		/* shift part i-times to the left
		   (equivalent of multiplying by BASE i-times */
		for(int shift_cnt = 0; shift_cnt < i; shift_cnt++)
			shift_left(part);
		
		add(&result, part);	
		destroy_BigNum(part);
		
		i++;
	}
	
	destroy_BigNum(A);
	destroy_BigNum(B);

	*a = result;
}

void subtract(BigNum_t** a, BigNum_t* b){
	BigNum_t* A = *a;
	BigNum_t* B = copy_BigNum(b);
	
	/* check validity of args */
	if(compare(A, B) == -1){
		printf("unable to subtract\n");
		exit(1);
	}
	
	/* normalize number lengths */
	BigNum_t* result = init_BigNum(A->size);
	add_leading_zeros(B, A->size);

	int i = 0;
	long long digit;
	while(i < A->size){
		digit = A->digits[i];
		
		/* borrow from higher rank digit */
		if(B->digits[i] > A->digits[i]){
			// borrow
			digit += BASE; 
			A->digits[i+1] -= 1;
		}
		
		/* update result */
		result->digits[result->size] = (int)digit - B->digits[i];
		result->size++;

		i += 1;
	}
	
	destroy_BigNum(A);
	destroy_BigNum(B);

	*a = result;
}

void divide_by_digit(BigNum_t** a, int b){
	BigNum_t* A = *a;
	
	BigNum_t* result = init_BigNum(A->size);
	result->size = A->size;
	
	int digit1, digit2, prev_digit, quotient;
	long long chunk, estimate, remainder;
	
	add_leading_zeros(A, A->size+1);
	int i = A->size-1;
	digit1 = A->digits[i];
	digit2 = A->digits[i-1];

	/* long division
	   (handling two-digit by one-digit number division chunks) */
	while(i > 0){
		
		/* calculate current digit */
		chunk = (long long)digit1 * BASE + digit2;
		quotient = (int) (chunk / b) ;	
		
		estimate = (long long)quotient * b;
		remainder = chunk - estimate;
		prev_digit = (int)(remainder % BASE);

		result->digits[i-1] = quotient;
		
		digit1 = prev_digit;
		digit2 = A->digits[i-2];
		
		i--;
	}
	
	destroy_BigNum(A);
	
	*a = result;
}

void divide(BigNum_t** a, BigNum_t* b) {
    BigNum_t* A = *a;
	
	if(b->size == 1){
		divide_by_digit(&A, b->digits[0]);
		*a = A;
		return;
	}
	/* multiply a and b by a factor
	   in order for first digit of b
	   to be close to BASE-1 (better estimations)*/
	int scale = 1;
	while(b->digits[b->size-1] > scale){
		scale *= 10;
	}
	scale = BASE / scale;
	multiply(&b, int_to_BigNum(scale));
	multiply(&A, int_to_BigNum(scale));
	
	BigNum_t* one = int_to_BigNum(1);
    BigNum_t* result = init_BigNum(A->size);
    BigNum_t* quotient;
	BigNum_t* estimate;
	
	
	/* extract (b->size+1) digits from a) */
	add_leading_zeros(A, A->size + 1);
	BigNum_t* part = copy_BigNum(A);
	int j = 0;
	while(j < (A->size-b->size-1)){
		shift_right(part);
		j++;
	}
	int i = A->size-1-b->size;
	result->size = i+1;

	/* long division */
	while(i >= 0){
		
		/* estimate single digit of result
		   by considering only first digit of b */
		quotient = copy_BigNum(part);
		divide_by_digit(&quotient, b->digits[b->size-1]);
		j = 0;
		while(j < b->size-1){
			shift_right(quotient);
			j++;
		}

		estimate = copy_BigNum(b);
		multiply(&estimate, quotient);
		
		/* if estimation is larger that 
		   part considered part of a
		   - subtract from it */
		while(compare(estimate, part)==1){
			subtract(&quotient, one);
			
			subtract(&estimate, b);
		}
		subtract(&part, estimate);		
		
		/* adjust part for next step of division */
		shift_left(part);
		part->size--;
		if(i > 0)
			part->digits[0] = A->digits[i-1];

		
		result->digits[i] = quotient->digits[0]; 
		
		destroy_BigNum(quotient);
		destroy_BigNum(estimate);
		
		i--;
	}
	
	destroy_BigNum(part);
	destroy_BigNum(one);

	*a = result;
}

BigNum_t* fast_exp(BigNum_t* base, BigNum_t* exponent){
	// TODO: adjust for bigger exponents
	int n = exponent->digits[0]; 
	BigNum_t* y = int_to_BigNum(1);
	BigNum_t* x = copy_BigNum(base);
	
	BigNum_t* temp;

	while(n > 1){
		if(n % 2 == 1){
			multiply(&y, x);
			n -= 1;
		}

		multiply(&x, x);

		n /= 2;
	}

	multiply(&x, y);

	return x;
}

void exponentiate(BigNum_t** base, BigNum_t* exponent){
	if(compare(exponent, int_to_BigNum(0))==0)
		*base = int_to_BigNum(1);
	else
		*base = fast_exp(*base, exponent);
}
