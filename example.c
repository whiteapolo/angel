#include <stdio.h>
#include <string.h>
#include "angel.h"

void main(void)
{
	// init angel by giving a path to a config file
	if (agl_init("example.conf")) {
		puts("could not find file");
		exit(1);
	}


	// get a string
	char str_var[256];
	if (agl_get_string("var1", str_var))
		printf("%s\n", str_var);


	// get an int
	int int_var;
	if (agl_get_int("var4", &int_var))
		printf("%d\n", int_var);


	// free up the memory
	agl_destory();
}
