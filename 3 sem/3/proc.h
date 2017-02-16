#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_STR 256
#define FAILURE -2

typedef struct proc_t proc_t;
struct proc_t
{
	int pid;
	char name[MAX_STR];
};

int proc_ctor(proc_t* this, int pid, char* name);
void proc_dtor(proc_t* this);
