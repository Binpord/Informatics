#include "proc.h"

int proc_ctor(proc_t* this, int pid, char* name)
{
	this->pid = pid;

	int len = strlen(name) + 1;		//+1 for \0 sumb
	if(len > MAX_STR) {
		errno = ENAMETOOLONG;
		return FAILURE;
	}
	memcpy(this->name, name, len);
	
	return 0;
}

void proc_dtor(proc_t* this)
{
	this->pid = -1;
	this->name[0] = '\0';
}
