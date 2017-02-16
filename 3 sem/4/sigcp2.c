#include "sthead.h"

#define PART_SIZE 8 * 1024
static char buf[PART_SIZE];
int fDst = 0, bytecount = 0, bitcount = 0;
const char* pidPath = "par.pid";
int par_pid = 0;
#undef __DBG
#undef __LOG

void usage(char* prog_name);
int sigcp(char* dstPath);
void inthandler(int signal);
void usr1handler(int signal);
void usr2handler(int signal);

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Invalid arguments\n");
		usage(argv[0]);

		kill(par_pid, SIGUSR2);

		return BAD_INPUT;
	}

	return sigcp(argv[1]);
}

void usage(char* prog_name)
{
	fprintf(stderr, "Usage:\n"
		"\t%s DST_FILE\n", prog_name);
}

int sigcp(char* dstPath)
{
	sigset_t mask, maskint;
	int stat = 0, ret = 0, tmp = 0;
	FILE* fPid = 0;

#ifdef __DBG
	FILE* pidfile = fopen("sigcp2.pid", "w");
	fprintf(pidfile, "%d", getpid());
	fclose(pidfile);
#endif

	stat = sigfillset(&mask);
	if(stat < 0)
	{
		fprintf(stdout, "Can't fill mask.\n");

		ret = FAILURE;
		goto ext;
	}

#ifdef __DBG
	sigdelset(&mask, SIGSTOP);
#endif

	stat = sigprocmask(SIG_BLOCK, &mask, NULL);
	if(stat < 0)
	{
		fprintf(stderr, "Can't set mask: %s\n", strerror(errno));
		
		ret = FAILURE;
		goto ext;
	}

#ifdef __LOG
	printf("Child:" "\t" "time = %li:" "\t" "Child blocked signals.\n", time(NULL));
#endif

	fPid = fopen(pidPath, "r");
	if(fPid < 0)
	{
		fprintf(stderr, "Can't open .pid file: %s\n", strerror(errno));
	
		ret = FAILURE;
		goto ext;
	}
	
	fscanf(fPid, "%d", &par_pid);

	fclose(fPid);
	fPid = 0;

	fDst = open(dstPath, O_WRONLY, 0666);
	if(fDst < 0)
	{
		fprintf(stderr, "Can't open destination file: %s\n", strerror(errno));
	
		ret = FAILURE;
		goto ext;
	}

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

	sa.sa_handler = inthandler;

	stat = sigaction(SIGINT, &sa, &sa_old);
	if(stat < 0)
	{
		fprintf(stderr, "Can't use sigaction: %s\n", strerror(errno));

		ret = FAILURE;
		goto ext;
	}

	memset(buf, 0, PART_SIZE);

	int first = true;

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

	stat = sigdelset(&mask, SIGINT);
	if(stat < 0)
	{
		fprintf(stderr, "Can't modify mask.\n");

		ret = FAILURE;
		goto ext;
	}

#ifdef __DBG
	raise(SIGSTOP);
#endif

	while(bytecount >= 0)
	{
		if(bytecount == 0 && bitcount == 0 && first == false)
		{
			stat = write(fDst, buf, PART_SIZE);
			if(stat < 0)
			{
				fprintf(stderr, "Can't write in file: %s\n", strerror(errno));
			
				ret = FAILURE;
				goto ext;
			}
			
			memset(buf, 0, PART_SIZE);
		}
		
		stat = kill(par_pid, SIGUSR1);
		if(stat < 0)
		{
			fprintf(stderr, "Can't send signal to parent process: %s\n", strerror(errno));
				
			ret = FAILURE;
			goto ext;
		}

		#ifdef __LOG
			printf("Child:" "\t" "time = %li:" "\t" "Signal SIGUSR1 was sent to parent.\n", time(NULL));
		#endif

		
		#ifdef __LOG
			printf("Child:" "\t" "time = %li:" "\t" "Child is waiting for a signal.\n", time(NULL));
		#endif

		sigsuspend(&mask);
		
		first = false;
	}
	
	#ifdef __LOG
		printf("Child:" "\t" "time = %li:" "\t" "Child exited from while.\n", time(NULL));
	#endif

ext:
	if(fDst > 0)
		close(fDst);

	if(fPid > 0)
		fclose(fPid);

	if(ret != 0)
	{
		kill(par_pid, SIGUSR2);


		#ifdef __LOG
			printf("Child:" "\t" "time = %li:" "\t" "Signal SIGUSR2 was sent to parent.\n", time(NULL));
		#endif
	}

#ifdef __DBG
	remove(dstPath);
#endif

	return ret;
}

void inthandler(int signal)
{	
#ifdef __LOG
	printf("Child:" "\t" "time = %li:" "\t" "Signal SIGINT detected.\n", time(NULL));
#endif

	if(bitcount != 0)
	{
		fprintf(stderr, "Parent worked badly and haven't transfered integer number of bytes.\n");

		exit(FAILURE);
	}

	int stat = write(fDst, buf, bytecount);
	if(stat < 0)
	{
		fprintf(stderr, "Can't write in file: %s\n", strerror(errno));
	
		kill(par_pid, SIGUSR2);

		#ifdef __LOG
			printf("Child:" "\t" "time = %li:" "\t" "Signal SIGUSR2 was sent to parent.\n", time(NULL));
		#endif

		exit(FAILURE);
	}
	
	bytecount = -1;
}

void usr1handler(int signal)
{
#ifdef __LOG
	printf("Child:" "\t" "time = %li:" "\t" "Signal SIGUSR1 detected.\n", time(NULL));
#endif
	bitcount++;
	if(bitcount == 8)
	{
		bitcount = 0;
		bytecount++;
		if(bytecount == PART_SIZE)
			bytecount = 0;
	}
}

void usr2handler(int signal)
{
#ifdef __LOG
	printf("Child:" "\t" "time = %li:" "\t" "Signal SIGUSR2 detected.\n", time(NULL));
#endif
	char tmp = 1;
	tmp <<= bitcount;
	bitcount++;

	buf[bytecount] |= tmp;


	if(bitcount == 8)
	{
		bitcount = 0;
		bytecount++;		
		if(bytecount == PART_SIZE)
			bytecount = 0;
	}
}
