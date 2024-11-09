#define DEFAULT_CAPACITY 4
#define BASE 1000000000

#include "bignum.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

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

void divide_by_digit(BigNum_t** a, int b, int* mod){
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
		if(i-2 >= 0)
			digit2 = A->digits[i-2];
		
		i--;
	}
	
	*mod = prev_digit;

	destroy_BigNum(A);
	
	*a = result;
}

void divide(BigNum_t** a, BigNum_t* b, BigNum_t** modulo) {
	destroy_BigNum(*modulo);
	BigNum_t* A = copy_BigNum(*a);
	BigNum_t* B = copy_BigNum(b);
	
	if(compare(A, B) == -1){
		destroy_BigNum(*a);
		destroy_BigNum(B);
		
		*a = int_to_BigNum(0);
		*modulo = copy_BigNum(A);
		destroy_BigNum(A);
		return;
	}


	int* mod = malloc(sizeof(int));

	if(b->size == 1){
		divide_by_digit(&A, b->digits[0], mod);
		*modulo = int_to_BigNum(*mod);
		free(mod);
		destroy_BigNum(B);
		destroy_BigNum(*a);
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
	BigNum_t* SCALE = int_to_BigNum(scale);
	multiply(&B, SCALE);
	multiply(&A, SCALE);
	
	BigNum_t* one = int_to_BigNum(1);
    BigNum_t* result = init_BigNum(A->size);
	result->digits[0] = 0;
    BigNum_t* quotient;
	BigNum_t* estimate;
	
	
	/* extract (b->size+1) digits from a) */
	add_leading_zeros(A, A->size + 1);
	BigNum_t* part = copy_BigNum(A);
	int j = 0;
	while(j < (A->size-B->size-1)){
		shift_right(part);
		j++;
	}
	int i = A->size-1-B->size;
	// write digit in reverse and reverse at the end
	/* long division */
	while(i >= 0){
		/* estimate single digit of result
		   by considering only first digit of b */
		quotient = copy_BigNum(part);
		divide_by_digit(&quotient, B->digits[B->size-1], mod);
		j = 0;
		while(j < B->size-1){
			shift_right(quotient);
			j++;
		}

		estimate = copy_BigNum(B);
		multiply(&estimate, quotient);
		
		/* if estimation is larger that 
		   part considered part of a
		   - subtract from it */
		while(compare(estimate, part)==1){
			subtract(&quotient, one);
			
			subtract(&estimate, B);
		}
		subtract(&part, estimate);		
		
		/* adjust part for next step of division */
		shift_left(part);
		part->size--;
		if(i > 0)
			part->digits[0] = A->digits[i-1];

		
		result->digits[result->size++] = quotient->digits[0]; 

		destroy_BigNum(quotient);
		destroy_BigNum(estimate);
		
		i--;
	}
	/* reverse result */ // TODO: UNNECESSARY
	if(result->size==0)
		result->size = 1;
	i = 0;
	int temp;
	while( i < result->size/2){
		temp = result->digits[i];
		result->digits[i] = result->digits[result->size-1-i];
		result->digits[result->size-1-i] = temp;
		i++;
	}
	
	*modulo = copy_BigNum(part);
	shift_right(*modulo);
	destroy_BigNum(part);
	destroy_BigNum(one);
	destroy_BigNum(SCALE);
	
	divide_by_digit(modulo, scale, mod);
	free(mod);
	destroy_BigNum(*a);
	destroy_BigNum(A);
	destroy_BigNum(B);
	*a = result;
}

BigNum_t* fast_exp(BigNum_t** base, BigNum_t* exponent){
	// TODO: adjust for bigger exponents
	int n = exponent->digits[0]; 
	BigNum_t* y = int_to_BigNum(1);
	BigNum_t* x = copy_BigNum(*base);
	destroy_BigNum(*base);

	while(n > 1){
		if(n % 2 == 1){
			multiply(&y, x);
			n -= 1;
		}

		multiply(&x, x);

		n /= 2;
	}

	multiply(&x, y);
	
	destroy_BigNum(y);
	return x;
}

void exponentiate(BigNum_t** base, BigNum_t* exponent){
	BigNum_t* zero = int_to_BigNum(0);
	
	if(compare(exponent, zero)==0){
		BigNum_t* one = int_to_BigNum(1);
		destroy_BigNum(*base);
		*base = one;
	}
	else
		*base = fast_exp(base, exponent);
	
	destroy_BigNum(zero);
}


int get_value(char c){
	if(48 <= c && c <= 57)
		return c-48;
	if(65 <= c && c <= 70)
		return 10+c-65;
	
	printf("base error\n");
	exit(1);
}

char get_char(int n){
	if(0 <= n && n <= 9)
		return n+48;
	if(10 <= n && n <= 15)
		return 65+n-10;
	
	printf("base error\n");
	exit(1);
}


BigNum_t* convert_to_decimal(char* string, int base){
	BigNum_t* B = int_to_BigNum(base);
	BigNum_t* result = init_BigNum(DEFAULT_CAPACITY);
	BigNum_t* current = init_BigNum(DEFAULT_CAPACITY);
	BigNum_t* current_base = int_to_BigNum(1);
	int digit;

	int len = strlen(string);
	int i = 0;
	while(i < len){
		current = copy_BigNum(current_base);
		digit = get_value(string[len-i-1]);

		multiply(&current, int_to_BigNum(digit));

		add(&result, current);
		destroy_BigNum(current);

		multiply(&current_base, B);
		i++;
	}

	return result;
}

char* convert_from_decimal(BigNum_t* num, int base){
	BigNum_t* B = int_to_BigNum(base);
	BigNum_t* zero = int_to_BigNum(0);
	BigNum_t* modulo;
	int mod;
	char* result = malloc(1000000);
	
	int i = 0;
	while(compare(num, zero) == 1){
		divide(&num, B, &modulo);

		// modulo always one digit
		mod = modulo->digits[0];
		result[i] = get_char(mod);
		i++;
	}

	int length = strlen(result);
    for (int i = 0; i < length / 2; i++) {
        char temp = result[i];
        result[i] = result[length - i - 1];
        result[length - i - 1] = temp;
    }

	return result;
}















