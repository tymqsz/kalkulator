#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "bignum.h"
#include "file_io.h"
#include "calc.h"


int main(int argc, char** argv){
	BigNum_t* a;
	BigNum_t* b;
	load_args_from_file(&a, &b, "input.txt");
	

	//print_BigNum(a);
	//print_BigNum(b);
	BigNum_t* c = multiply(a, b);
	
	print_BigNum(c);

	return 0;
}

