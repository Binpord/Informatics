//Made by Vadim Shiyanov.

#include "sthead.h"
#include "list.h"

#define MAX_STR 256

const int BAD_CHILD = -3;
const char* INIT_STR = "vs-bash:~$ ";

char** make_argv(char* str);
int ml_bash();
int findarg(char smbl, char** args);
int execute(char** newargv, int newinfd, list_t* procs);
char** separgs(char** newargv, int count);
int count_args(char** start);
void take_peace(char** dst, char** src);

int main()
{
	return ml_bash();
}

int ml_bash()
{
	int pid = 0, stat = 0, check = 0, ret = 0, fOut = 0, count = 0;
	static char input[MAX_STR];
	static char buf[MAX_STR];
	list_t procs;
	proc_t proc;

	list_ctor(&procs);
	
	while(true)
	{
		check = write(STDOUT_FILENO, INIT_STR, strlen(INIT_STR));
		if(check != strlen(INIT_STR))
		{
			fprintf(stderr, "Can't write on console: %s\n", strerror(errno));

			ret = FAILURE;
			goto ext;
		}

		check = read(STDIN_FILENO, input, MAX_STR);
		if(check < 0)
		{
			fprintf(stderr, "Can't read from console: %s\n", strerror(errno));

			ret = FAILURE;
			goto ext;
		}

		input[check - 1] = '\0';				//for changing '\n' to '\0'

		if(!strlen(input))
			continue;
	
		if(!strcmp(input, "q"))
			break;
		
		char** newargv = make_argv(input);
		if(newargv == NULL)
		{
			fprintf(stderr, "Can't parse args: %s\n", strerror(errno));

			ret = FAILURE;
			goto ext;
		}

		fOut = execute(newargv, STDIN_FILENO, &procs);
		if(fOut < 0)
		{
			fprintf(stderr, "Can't run executing function: %s\n", strerror(errno));
			errno = 0;
		}

		stat = read(fOut, buf, MAX_STR);
		while(stat > 0)
		{
			count += stat;
			stat = write(STDOUT_FILENO, buf, stat);
			if(stat < 0)
			{
				fprintf(stderr, "Can't write on console: %s\n", strerror(errno));
				
				ret = FAILURE;
				goto ext;
			}

			stat = read(fOut, buf, MAX_STR);
		}
		if(stat < 0)
		{
			fprintf(stderr, "Can't read from pipe: %s\n", strerror(errno));
		
			ret = FAILURE;
			goto ext;
		}

		while(procs.size > 0) 
		{
			pid = wait(&stat);
			if(pid == -1) 
			{
				fprintf(stderr, "Can't detect child: %s\n", strerror(errno));

				ret = FAILURE;
				goto ext;
			}
			else if(pid > 0) 
			{
				check = list_cut(&procs, &proc, pid);
				if(check == FAILURE) 
				{
					fprintf(stderr, "Warning: Can't find process in list. List was corrupted.\n");
					ret = FAILURE;
					goto ext;
				}

				fprintf(stdout, "Process %s finished with return code %d.\n", proc.name, WEXITSTATUS(stat));

				proc_dtor(&proc);
			}
		}
		
		fprintf(stdout, "Process's output was %d.\n", count);
		count = 0;
	}

ext:
	list_dtor(&procs);
	
	if(fOut > 0)
	{
		close(fOut);
	}
	
	return ret;
}

char** make_argv(char* str)
{
	assert(str);
	
	char* s_word = NULL;

	//counting number of args
	int count = 0;
	for(int i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == ' ')
			count++;
	}
	count += 2;				//for 1 uncounted ' ' and 1 for NULL at the end

	char** res = malloc(count * sizeof(char*));
	if(res == NULL) 
	{
		//errno was set
		return NULL;	
	}
	
	
	res[0] = strtok_r(str, " ", &s_word);
	for(int i = 1; i < count - 1; i++) 
	{
		res[i] = strtok_r(NULL, " ", &s_word);
	}
	
	res[count - 1] = NULL;

	return res;
}

int findarg(char smbl, char** args)		//returns number of first arg, which contains smbl
{						//expects NULL ptr as the last arg
	assert(args);				//returns -1 if smbl wasn't detected
	
	int i = 0;
	int j = 0;

	for(i; args[i] != NULL; i++)
	{
		for(j = 0; args[i][j] != '\0'; j++)
		{
			if(args[i][j] == smbl)
				break;
		}

		if(args[i][j] == smbl)
			break;
	}
	
	if(args[i] != NULL)
		if(args[i][j] == smbl)
			return i;

	return -1;
}

int execute(char** newargv, int newinfd, list_t* procs)
{
	assert(newargv);

	int pid = 0;
	int fd[2];
	int stat = 0;
	
	stat = findarg('|', newargv);
	if(stat < 0)				//returns -1 if smbl wasn't detected
	{
		stat = pipe(fd);
		if(stat < 0)
			return FAILURE;

		pid = fork();
		if(pid < 0)
		{
			//errno was set
			return FAILURE;
		}
		else if(pid == 0)
		{	
			if(newinfd != STDIN_FILENO)
			{
				stat = dup2(newinfd, STDIN_FILENO);
				if(stat < 0)
				{
					fprintf(stderr, "Can't dup file descriptors: %s\n", strerror(errno));
					exit(BAD_CHILD);
				}
			}

			stat = dup2(fd[1], STDOUT_FILENO);
			if(stat < 0)
			{
				fprintf(stderr, "Can't dup file descriptors: %s\n", strerror(errno));
				exit(BAD_CHILD);
			}

			execvp(newargv[0], newargv);
			
			//never here
			fprintf(stderr, "Can't execute program <%s>: %s\n", newargv[0], strerror(errno));

			exit(BAD_CHILD);
		}
		
		list_add(procs, pid, newargv[0]);
		close(fd[1]);
	
		if(newinfd != STDIN_FILENO)
			close(newinfd);

		return fd[0];
	}

	char** thrdargv = separgs(newargv, stat);
	if(thrdargv == NULL)
	{
		//errno was set
		return FAILURE;
	}

	stat = pipe(fd);
	if(stat < 0)
	{
		//errno was set
		return FAILURE;
	}
		

	pid = fork();
	if(pid < 0)
	{
		//errno was set
		return FAILURE;
	}
	else if(pid == 0)
	{
		if(newinfd != STDIN_FILENO)
		{
			stat = dup2(newinfd, STDIN_FILENO);
			if(stat < 0)
			{
				fprintf(stderr, "Can't dup file descriptors: %s\n", strerror(errno));
				exit(BAD_CHILD);
			}
		}

		stat = dup2(fd[1], STDOUT_FILENO);
		if(stat < 0)
		{
			fprintf(stderr, "Can't dup file descriptors: %s\n", strerror(errno));
			exit(BAD_CHILD);
		}

		execvp(newargv[0], newargv);
		
		//never here
		fprintf(stderr, "Can't execute program <%s>: %s\n", newargv[0], strerror(errno));

		exit(BAD_CHILD);
	}

	list_add(procs, pid, newargv[0]);
	free(newargv);

	if(newinfd != STDIN_FILENO)
		close(newinfd);

	close(fd[1]);

	return execute(thrdargv, fd[0], procs);
}

char** separgs(char** newargv, int count)
{
	assert(newargv);

	int stat = 0;
	
	char** res;
	
	char* susparg = newargv[count];

	int i = 0;
	for(i; susparg[i] != '\0'; i++)
	{
		if(susparg[i] == '|')
			break;
	}
	
	if(susparg[i] == '\0')
	{
		errno = EINVAL;
		return NULL;
	}

	int len = strlen(susparg);

	if(i == 0)
	{
		if(len == 1)
		{
			res = malloc(count_args(newargv + count + 1) * sizeof(char*));
			take_peace(res, newargv + count + 1);

			newargv[count] = NULL;

			return res;
		}
		
		newargv[count] += 1;
		res = malloc(count_args(newargv + count) * sizeof(char*));
		take_peace(res, newargv + count);
		
		newargv[count] = NULL;
		
		return res;
	}
	
	if(i == (len - 1))
	{
		res = malloc(count_args(newargv + count + 1) * sizeof(char*));
		take_peace(res, newargv + count + 1);
		
		newargv[count][len-1] = '\0';
		newargv[count + 1] = NULL;

		return res;
	}
	
	res = malloc(count_args(newargv + count) * sizeof(char*));

	res[0] = newargv[count] + i + 1;

	take_peace(res + 1, newargv + count + 1);
	
	newargv[count][i] = '\0';
	newargv[count + 1] = NULL;

	return res;
}

int count_args(char** start)
{
	int count = 0;
	
	while(start[count] != NULL)
		count++;

	count++;

	return count;
}

void take_peace(char** dst, char** src)
{
	int i = 0;

	for(i; src[i] != NULL; i++)
	{
		dst[i] = src[i];
	}
	
	dst[i] = src[i];		//for dst[last] = NULL
}
