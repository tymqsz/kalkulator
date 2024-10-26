#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "bignum.h"
#include "file_io.h"
#include "calc.h"


int main(int argc, char** argv){
	BigNum_t* a;
	BigNum_t* b;
	BigNum_t* c;
	
	load_args_from_file(&a, &b, "input.txt");
	
	c = add(a, b);
	print_BigNum(c);
	destroy_BigNum(c);
	
	printf("--------------------------\n\n\n");
	c = multiply(a, b);
	print_BigNum(c);
	destroy_BigNum(c);
	
	printf("--------------------------\n\n\n");
	c = divide(a, b);
	print_BigNum(c);
	destroy_BigNum(c);
	
	printf("--------------------------\n\n\n");
	destroy_BigNum(a);
	destroy_BigNum(b);

	return 0;
}

