#include "sthead.h"

// открывает log-файл, ожидает глобальную переменную FILE* fLog
int openlog(char* procname)
{
	int ret = 0;
	size_t size = strlen(procname) + sizeof(char) * 5;
	char* logpath = malloc(size);		// +4 для приписывания ".log"
	if(!logpath)
	{
		fprintf(stderr, "Can't allocate memory: %s\n", strerror(errno));
		ret = FAILURE;
		goto openlogext;
	}

	snprintf(logpath, size, "%s.log", procname);

	fLog = fopen(logpath, "w+");
	if(fLog == NULL)
	{
		fprintf(stderr, "Can't create log-file: %s\n", strerror(errno));
		ret = FAILURE;
	}

openlogext:

	if(logpath)
	{
		free(logpath);
	}

	return ret;
}
