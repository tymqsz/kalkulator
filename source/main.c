#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "file_io.h"

#define MAX_FILENAME 100


int main(int argc, char** argv){
	if(argc == 1){
		fprintf(stderr, "nie podano pliku wejsciowego\n");
		return 1;
	}
	
	/* wczytanie argumentow, ustalenie plikow wejscia i wyjscia */
	char* input_filename = argv[1];
	char* output_filename = malloc(MAX_FILENAME);
	if(argc > 2)
		strcpy(output_filename,argv[2]);
	else{
		strcpy(output_filename, "out_");
		strcat(output_filename, input_filename);
	}
	
	/* wykonanie dzialan */
	process_input_file(input_filename, output_filename);
	
	free(output_filename);
	return 0;

}
