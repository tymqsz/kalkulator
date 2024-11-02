#ifndef C
#define C

void add(BigNum_t**, BigNum_t*);

void multiply(BigNum_t**, BigNum_t*);

void exponentiate(BigNum_t**, BigNum_t*);

void subtract(BigNum_t**, BigNum_t*);

void divide(BigNum_t**, BigNum_t*, BigNum_t**);

void divide_by_digit(BigNum_t**, int, int*);

BigNum_t* convert_to_decimal(char*, int);

char* convert_from_decimal(BigNum_t*, int);
#endif
