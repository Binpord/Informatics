#include "sthead.h"
#include <math.h>

#define PART_SIZE 8 * 1024
#undef __LOG
const char* secProg = "sigcp2";
const char* pidPath = "par.pid";
static char buf[PART_SIZE];
int fSrc = 0, bytecount = 0, bitcount = 0, rd = 0, pid = 0, end = false;

FILE* fLog = NULL;
#include "sthead.c"

void usage(char* prog_name);
int sigcp(char* srcPath, char* dstPath);
void usr1handler(int signal);
void usr2handler(int signal);

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		fprintf(stderr, "Invalid arguments\n");
		usage(argv[0]);
		return BAD_INPUT;
	}

	return sigcp(argv[1], argv[2]);
}

void usage(char* prog_name)
{
	fprintf(stderr, "Usage:\n"
		"\t%s SRC_FILE DST_FILE\n", prog_name);
}

int sigcp(char* srcPath, char* dstPath)
{
	sigset_t mask;
	int stat, ret = 0, fDst = 0, tmp = 0;
	FILE* fPid = NULL;

	stat = sigfillset(&mask);
	if(stat < 0)
	{
		fprintf(stdout, "Can't fill mask.\n");

		ret = FAILURE;
		goto ext;
	}

	stat = sigprocmask(SIG_BLOCK, &mask, NULL);
	if(stat < 0)
	{
		fprintf(stderr, "Can't set mask: %s\n", strerror(errno));
		
		ret = FAILURE;
		goto ext;
	}

#ifdef __LOG
	printf("Parent:" "\t" "time = %li:" "\t" "Blocked signals.\n", time(NULL));
#endif

	fPid = fopen(pidPath, "w");
	if(fSrc < 0)
	{
		fprintf(stderr, "Can't open pid file: %s\n", strerror(errno));
	
		ret = FAILURE;
		goto ext;
	}

	stat = fprintf(fPid, "%d", getpid());
	if(stat < 0)
	{
		fprintf(stderr, "Can't write in .pid file: %s\n", strerror(errno));
		
		ret = FAILURE;
		goto ext;
	}
	
	fclose(fPid);
	fPid = 0;

	fSrc = open(srcPath, O_RDONLY);
	if(fSrc < 0)
	{
		fprintf(stderr, "Can't open source file: %s\n", strerror(errno));
	
		ret = FAILURE;
		goto ext;
	}

	fDst = open(dstPath, O_CREAT | O_EXCL, 0666);
	if(fDst < 0)
	{
		fprintf(stderr, "Can't open destination file: %s\n", strerror(errno));
	
		ret = FAILURE;
		goto ext;
	}
	close(fDst);
	fDst = 0;

	pid = fork();
	if(pid < 0)
	{
		fprintf(stderr, "Can't fork process: %s\n", strerror(errno));
		
		ret = FAILURE;
		goto ext;
	}
	else if(pid == 0)
	{
		execl(secProg, secProg, dstPath, NULL);

		fprintf(stderr, "Can't execute 2nd programm: %s\n", strerror(errno));
		exit(FAILURE);
	}

#ifdef __LOG
	printf("Parent:" "\t" "time = %li:" "\t" "Child made.\n", time(NULL));
#endif


	struct sigaction sa, sa_old;
	memset(&sa, 0, sizeof(sa));
	
	sa.sa_handler = usr1handler;
	sa.sa_mask = mask;
	
	stat = sigaction(SIGUSR1, &sa, &sa_old);
	if(stat < 0)
	{
		fprintf(stderr, "Can't use sigaction: %s\n", strerror(errno));

		ret = FAILURE;
		goto ext;
	}
	
	sa.sa_handler = usr2handler;

	stat = sigaction(SIGUSR2, &sa, &sa_old);
	if(stat < 0)
	{
		fprintf(stderr, "Can't use sigaction: %s\n", strerror(errno));

		ret = FAILURE;
		goto ext;
	}

	stat = sigfillset(&mask);
	if(stat < 0)
	{
		fprintf(stderr, "Can't modify mask.\n");

		ret = FAILURE;
		goto ext;
	}

	stat = sigdelset(&mask, SIGUSR1);
	if(stat < 0)
	{
		fprintf(stderr, "Can't modify mask.\n");

		ret = FAILURE;
		goto ext;
	}

	stat = sigdelset(&mask, SIGUSR2);
	if(stat < 0)
	{
		fprintf(stderr, "Can't modify mask.\n");

		ret = FAILURE;
		goto ext;
	}

	while(true)
	{
		if(bytecount < 0)
		{
			tmp = wait(&stat);

			fprintf(stderr, "Child worked badly and exited with return code %d.\n", WEXITSTATUS(stat));
			pid = 0;

			ret = FAILURE;
			goto ext;
		}
		if(bytecount == 0 && bitcount == 0)
		{
			rd = read(fSrc, buf, PART_SIZE);
			if(rd < 0)
			{
				fprintf(stderr, "Can't read from file <%s>: %s\n", srcPath, strerror(errno));
			
				ret = FAILURE;
				goto ext;
			}
			if(!rd)
				break;
		}
		
		tmp = waitpid(pid, &stat, WNOHANG);
		if(tmp == pid)
		{
			fprintf(stderr, "Child exited earlier than the copy ended with exit code %d.\n", WEXITSTATUS(stat));
			pid = 0;

			ret = FAILURE;
			goto ext;
		}

		
		#ifdef __LOG
			printf("Parent:" "\t" "time = %li:" "\t" "Parent is waiting for a signal.\n", time(NULL));
		#endif

		sigsuspend(&mask);
	}
	
	#ifdef __LOG
		printf("Parent:" "\t" "time = %li:" "\t" "Parent exited from while.\n", time(NULL));
	#endif

	end = true;
	sigsuspend(&mask);

	stat = kill(pid, SIGINT);
	if(stat < 0)
	{
		fprintf(stderr, "Can't send signal to child process: %s\n", strerror(errno));
			
		ret = FAILURE;
		goto ext;
	}


	#ifdef __LOG
		printf("Parent:" "\t" "time = %li:" "\t" "Signal SIGINT was sent to a child.\n", time(NULL));
	#endif

ext:
	if(fSrc > 0)
		close(fSrc);

	if(fDst > 0)
		close(fDst);

	if(fPid > 0)
	{
		fclose(fPid);
		remove(pidPath);
	}
	
	if(pid > 0)
	{
		wait(&stat);
	
		fprintf(stdout, "Child exited with return code %d.\n", WEXITSTATUS(stat));
		pid = 0;
	}
	
	return ret;
}

void usr1handler(int signal)
{
	if(end)
		return;

#ifdef __LOG
	printf("Parent:" "\t" "time = %li:" "\t" "Signal SIGUSR1 detected.\n", time(NULL));
#endif

	int stat;
	char tmp = buf[bytecount];

	tmp >>= bitcount;
	bitcount++;
	if(bitcount == 8)
	{
		bitcount = 0;
		bytecount++;
		if(bytecount == rd)
			bytecount = 0;
	}

	tmp &= 1;

	if(tmp)
	{
		stat = kill(pid, SIGUSR2);
		if(stat < 0)
		{
			fprintf(stderr, "Can't send signal to child process: %s\n", strerror(errno));
			
			exit(FAILURE);
		}

		#ifdef __LOG
			printf("Parent:" "\t" "time = %li:" "\t" "Signal SIGUSR2 was sent to a child.\n", time(NULL));
		#endif
	
		return;
	}
	
	stat = kill(pid, SIGUSR1);
	if(stat < 0)
	{
		fprintf(stderr, "Can't send signal to child process: %s\n", strerror(errno));
			
		exit(FAILURE);
	}

	#ifdef __LOG
		printf("Parent:" "\t" "time = %li:" "\t" "Signal SIGUSR1 was sent to a child.\n", time(NULL));
	#endif

	return;
}

void usr2handler(int signal)
{
#ifdef __LOG
	printf("Parent:" "\t" "time = %li:" "\t" "Signal SIGUSR2 detected.\n", time(NULL));
#endif

	bytecount = -1;
}
