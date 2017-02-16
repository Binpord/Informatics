#include "proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAILURE -2

typedef struct listobj_t listobj_t;
struct listobj_t
{
	proc_t* info;
	listobj_t* next;
};

typedef struct list_t list_t;
struct list_t
{
	listobj_t* first;
	int size;
};

int list_ctor(list_t* this);
int list_add(list_t* this, int pid, char* name);
int list_cut(list_t* this, proc_t* dst, int pid);
int listobj_ctor(listobj_t* this, int pid, char* name);
listobj_t* list_findlast(list_t* this);
void listobj_dtor(listobj_t* this);
void list_dtor(list_t* this);
