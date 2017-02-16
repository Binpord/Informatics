#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FAILURE -2

typedef struct argv_t argv_t;
struct argv_t
{
	char** args;
	int len;		//for correct destruction
};

int argv_ctor(argv_t*, char*, char*);
void argv_dtor(argv_t*);
