#ifndef C
#define C

BigNum_t* add(BigNum_t*, BigNum_t*);

BigNum_t* multiply(BigNum_t*, BigNum_t*);

BigNum_t* exponentiate(BigNum_t*, BigNum_t*);

int divide_by_digit(int, int, int);

BigNum_t* subtract(BigNum_t*, BigNum_t*);

BigNum_t* divide(BigNum_t*, BigNum_t*);

BigNum_t* single_divide(BigNum_t*, int);
#endif
