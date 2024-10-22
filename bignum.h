#ifndef BN
#define BN

typedef struct{
	int* digits;
	int capacity;
	int size;
} BigNum_t;

BigNum_t* init_BigNum(int);

void destroy_BigNum(BigNum_t*);

BigNum_t* copy_BigNum(BigNum_t* num);

void expand_BigNum(BigNum_t*, int);

void shift_left(BigNum_t*);

BigNum_t* string_to_BigNum(char*);

void print_BigNum(BigNum_t*);

void add_leading_zeros(BigNum_t*, int);



#endif
