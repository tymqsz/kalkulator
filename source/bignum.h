#ifndef BIGNUM_H
#define BIGNUM_H

typedef struct{
	int* digits;
	int capacity;
	int size;
} BigNum_t;

char* BigNum_to_string(BigNum_t*);

BigNum_t* init_BigNum(int);

void destroy_BigNum(BigNum_t*);

BigNum_t* copy_BigNum(BigNum_t* num);

void expand_BigNum(BigNum_t*, int);

void shift_left(BigNum_t*);

void shift_right(BigNum_t*);

BigNum_t* string_to_BigNum(char*);

BigNum_t* int_to_BigNum(int);

void add_leading_zeros(BigNum_t*, int);

void remove_leading_zeros(BigNum_t*);

int compare(BigNum_t*, BigNum_t*);

#endif
