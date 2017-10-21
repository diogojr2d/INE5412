#include <stdio.h>

int main(int argc, char *argv[]) {
	printf("char: %lu\n", sizeof(char)*8 );
	printf("short int: %lu\n", sizeof(short int)*8 );
	printf("int: %lu\n", sizeof(int)*8 );
	printf("long int: %lu\n", sizeof(long int)*8 );
	printf("float: %lu\n", sizeof(float)*8 );
	printf("double: %lu\n", sizeof(double)*8 );

	return 0;
}