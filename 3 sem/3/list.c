#include "list.h"

int list_ctor(list_t* this)
{
	this->first = NULL;
	this->size = 0;
	return 0;
}

int list_add(list_t* this, int pid, char* name)
{
	listobj_t* proc = malloc(sizeof(listobj_t*));
	if(!proc) {
		fprintf(stderr, "Can't allocate %lu bytes of memory.\n", sizeof(listobj_t*));
		return FAILURE;
	}

	int stat = listobj_ctor(proc, pid, name);
	if(stat == FAILURE) {
		fprintf(stderr, "Can't create list object.\n");
		return FAILURE;
	}

	if(this->size == 0) {
		this->first = proc;
		this->size++;
		return 0;
	}
	
	listobj_t* last = list_findlast(this);
	if(last == NULL) {
		fprintf(stderr, "Can't find list's last object. List was corrupted.\n");
		return FAILURE;
	}
	
	last->next = proc;
	this->size++;
	
	return 0;
}

int list_cut(list_t* this, proc_t* dst, int pid)
{
	listobj_t* prev = NULL;
	listobj_t* now = this->first;
	if(now == NULL) {
		fprintf(stderr, "Warning: attempt to cut from empty list was detected. Something went wrong.");
		return FAILURE;
	}

	int count = 1;
	
	while(now->info->pid != pid && count <= this->size) {
		prev = now;
		now = now->next;
		count++;
	}

	if(now->info->pid == pid) {
		if(prev == NULL) {
			this->first = now->next;
			this->size--;

			//memmove(now->info, dst, sizeof(proc_t));		Why free error!!!
			*dst = *(now->info);
			listobj_dtor(now);
			free(now);
			
			return 0;
		}
		
		prev->next = now->next;
		this->size--;

		memmove(dst, now->info, sizeof(proc_t));

		listobj_dtor(now);
		free(now);

		return 0;
	}
	
	return FAILURE;
}

int listobj_ctor(listobj_t* this, int pid, char* name)
{	
	proc_t* proc = malloc(sizeof(proc_t*));
	if(!proc) {
		fprintf(stderr, "Can't allocate %lu bytes of memory.\n", sizeof(proc_t*));
		return FAILURE;
	}

	int stat = proc_ctor(proc, pid, name);
	if(stat == FAILURE) {
		fprintf(stderr, "Can't create structure proc_t.\n");
		return FAILURE;
	}

	this->info = proc;
	this->next = NULL;

	return 0;
}

listobj_t* list_findlast(list_t* this)
{
	listobj_t* now = this->first;
	int count = 1;
	
	while(now->next != NULL && count <= this->size) {
		now = now->next;
		count++;
	}

	if(now->next == NULL && count == this->size) {
		return now;
	}
	
	return NULL;
}

void listobj_dtor(listobj_t* this)
{
	proc_dtor(this->info);
	free(this->info);
	this->info = NULL;

	this->next = NULL;
}

void list_dtor(list_t* this)
{
	listobj_t* prev = NULL;
	listobj_t* now = this->first;
	if(now == NULL) {
		this->size = -1;

		return;
	}
	
	int count = 1;

	while(now->next != NULL && count <= this->size) {
		prev = now;
		now = now->next;
		listobj_dtor(prev);
		free(prev);
	}
	
	listobj_dtor(now);
	free(now);
	
	this->size = -1;
	this->first = NULL;
}
