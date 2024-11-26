#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "bignum.h"

#define MAX_STRING_LEN 1000000

/* initialize a BigNum with given capacity
   (capacity of n - storing n digits base 10^9 */
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

/* extend BigNums capacity to target */
void expand_BigNum(BigNum_t* num, int target_capacity){
	num->capacity = target_capacity;
	
	num->digits = realloc(num->digits, sizeof(int)*target_capacity);

	for(int i = num->size; i < num->capacity; i++)
		num->digits[i] = 0;
}

/* shift all digits by one to left
   equivalent to multiplying by 10^9 */
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

/* equivalent to division by 10^9 */
void shift_right(BigNum_t* num){
	num->size--;
	
	int i = 0;
	while(i < num->size){
		num->digits[i] = num->digits[i+1];
		i++;
	}

	num->digits[num->size] = 0;
}

void remove_leading_zeros(BigNum_t* num){
	while(num->size > 1 && num->digits[num->size-1] == 0){
		num->size--;
	}
}

void add_leading_zeros(BigNum_t* num, int target_size){
	while(num->size < target_size){
		if(num->size+1 >= num->capacity)
			expand_BigNum(num, num->capacity*2);
		
		num->digits[num->size++] = 0;
	}
}

/* compare to BigNums:
   -1 -> a < b
   	0 -> a = b
	1 -> a > b */
int compare(BigNum_t* a, BigNum_t* b){
	int result = 0;

	/* before comparing remove leading zeros */
	BigNum_t* a_copy = copy_BigNum(a);
	BigNum_t* b_copy = copy_BigNum(b);
	remove_leading_zeros(a_copy);
	remove_leading_zeros(b_copy);
	
	/* compare sizes */
	if(a_copy->size > b_copy->size){
		result = 1;
		goto END;
	}
	if(a_copy->size < b_copy->size){
		result = -1;
		goto END;
	}
	
	/* compare digits one by one */
	int i = a_copy->size-1;
	while(i >= 0){
		if(a_copy->digits[i] > b_copy->digits[i]){
			result = 1;
			break;
		}
		if(a_copy->digits[i] < b_copy->digits[i]){
			result = -1;
			break;
		}
		i--;
	}

	END:
	destroy_BigNum(a_copy);
	destroy_BigNum(b_copy);
	return result;
}

/* deepcopy of num */
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

/* create single digit BigNum */
BigNum_t* int_to_BigNum(int n){
	BigNum_t* num = init_BigNum(1);
	num->digits[num->size++] = n;

	return num;
}

BigNum_t* string_to_BigNum(char* string) {
    BigNum_t* num = init_BigNum(4); 
    
    int len = strlen(string);
    int start, size;
    char* digit = malloc(10); /* 9 digits + '\0' */
    int i = len;

    while (i > 0) {
		/* if possible take 9 digits,
		   otherwise the most you can */
        start = (i - 9) >= 0 ? (i - 9) : 0;
        size = i - start;
		
		/* copy those digits */
        //digit = malloc(size + 1); // TODO: no need to alloc
        strncpy(digit, string + start, size);
        digit[size] = '\0';

		/* check if more space needed */
        if (num->size + 1 >= num->capacity) {
            expand_BigNum(num, num->capacity + 2);
        }
		
		/* load digit */
        num->digits[num->size++] = atoi(digit);

        i -= 9;
    }

    free(digit);
    return num;
}

char* BigNum_to_string(BigNum_t* num){
	remove_leading_zeros(num);	
	char* result = malloc(MAX_STRING_LEN);
	
	/* load at least first '0' */
	sprintf(result, "%d", num->digits[num->size-1]);
	
	int i;
	char part[10];
	for (i = num->size-2; i >= 0; i--) {
        /* load one (base 10^9) digit */
		sprintf(part, "%09d", num->digits[i]);
        
        if (strlen(result) + strlen(part) + 1 < MAX_STRING_LEN) {
            strcat(result, part);
        } else {
            fprintf(stderr, "przekroczono max. dlugosc string przy konwersji z BigNum\n");
            break;
        }
    }

	return result;
}
