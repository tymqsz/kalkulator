#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "ctype.h"
#include "bignum.h"
#include "file_io.h"
#include "calc.h"

#define MAX_LINE 1000000
#define MAX_ARGS 100

#define OUT "../assets/c_out.txt"


int main(int argc, char** argv){
	char filename[] = "../assets/input.txt";
	
	process_input_file(filename);
		
	return 0;

}
