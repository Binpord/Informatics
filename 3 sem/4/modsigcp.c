#include "sthead.h"
#include <math.h>

#define PART_SIZE 8 * 1024
#undef __LOG
#undef __DBG
static char buf[PART_SIZE];
int fSrc = 0, fDst = 0, chld_pid = 0, par_pid = 0, end = false;
int bytecount = 0, bitcount = 0, rd = 0;

void usage(char* prog_name);
int sigcp(char* srcPath, char* dstPath);
int send();
int recieve();
void send_usr_handler(int signal);
void recieve_usr_handler(int signal);
void inthandler(int signal);

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
	int stat = 0, ret = 0, tmp = 0;

	stat = sigfillset(&mask);
	if(stat < 0)
	{
		fprintf(stdout, "Can't fill mask.\n");

		ret = FAILURE;
		goto sigcp_ext;
	}

	stat = sigprocmask(SIG_BLOCK, &mask, NULL);
	if(stat < 0)
	{
		fprintf(stderr, "Can't set mask: %s\n", strerror(errno));
		
		ret = FAILURE;
		goto sigcp_ext;
	}

#ifdef __LOG
	printf("Parent:" "\t" "time = %li:" "\t" "Blocked signals.\n", time(NULL));
#endif

	par_pid = getpid();

#ifdef __DBG
	FILE* fPid = fopen("par.pid", "w");
	fprintf(fPid, "%d", par_pid);
	fclose(fPid);
	fPid = 0;
#endif

	fSrc = open(srcPath, O_RDONLY);
	if(fSrc < 0)
	{
		fprintf(stderr, "Can't open source file: %s\n", strerror(errno));
	
		ret = FAILURE;
		goto sigcp_ext;
	}

	fDst = open(dstPath, O_CREAT | O_EXCL, 0666);
	if(fDst < 0)
	{
		fprintf(stderr, "Can't open destination file: %s\n", strerror(errno));
	
		ret = FAILURE;
		goto sigcp_ext;
	}

	close(fDst);
	fDst = 0;

	chld_pid = fork();
	if(chld_pid < 0)
	{
		fprintf(stderr, "Can't fork process: %s\n", strerror(errno));
		
		ret = FAILURE;
		goto sigcp_ext;
	}
	else if(chld_pid == 0)
	{
		fDst = open(dstPath, O_WRONLY, 0666);
		if(fDst < 0)
		{
			fprintf(stderr, "Can't open destination file: %s\n", strerror(errno));

			ret = FAILURE;
			kill(par_pid, SIGUSR2);
			goto sigcp_ext;
		}
		ret = recieve();

		goto sigcp_ext;
	}

#ifdef __LOG
	printf("Parent:" "\t" "time = %li:" "\t" "Child made.\n", time(NULL));
#endif

	ret = send();

sigcp_ext:

	if(fSrc > 0)
		close(fSrc);

	if(fDst > 0)
		close(fDst);
	
	if(chld_pid > 0)
	{
		wait(&stat);
	
		fprintf(stdout, "Child exited with return code %d.\n", WEXITSTATUS(stat));
		chld_pid = 0;
	}
	
	return ret;
}

int send()
{
	int stat = 0, ret = 0, tmp = 0;
	sigset_t mask;
	struct sigaction sa, sa_old;
	memset(&sa, 0, sizeof(sa));
	
	stat = sigfillset(&mask);
	if(stat < 0)
	{
		fprintf(stderr, "Can't modify mask.\n");

		ret = FAILURE;
		goto send_ext;
	}

	sa.sa_handler = send_usr_handler;
	sa.sa_mask = mask;
	
	stat = sigaction(SIGUSR1, &sa, &sa_old);
	if(stat < 0)
	{
		fprintf(stderr, "Can't use sigaction: %s\n", strerror(errno));

		ret = FAILURE;
		goto send_ext;
	}

	stat = sigaction(SIGUSR2, &sa, &sa_old);
	if(stat < 0)
	{
		fprintf(stderr, "Can't use sigaction: %s\n", strerror(errno));

		ret = FAILURE;
		goto send_ext;
	}

	stat = sigdelset(&mask, SIGUSR1);
	if(stat < 0)
	{
		fprintf(stderr, "Can't modify mask.\n");

		ret = FAILURE;
		goto send_ext;
	}

	stat = sigdelset(&mask, SIGUSR2);
	if(stat < 0)
	{
		fprintf(stderr, "Can't modify mask.\n");

		ret = FAILURE;
		goto send_ext;
	}

	while(true)
	{
		if(bytecount < 0)
		{
			wait(&stat);

			fprintf(stderr, "Child worked badly and exited with return code %d.\n", WEXITSTATUS(stat));
			chld_pid = 0;

			ret = FAILURE;
			goto send_ext;
		}
		if(bytecount == 0 && bitcount == 0)
		{
			rd = read(fSrc, buf, PART_SIZE);
			if(rd < 0)
			{
				fprintf(stderr, "Can't read from file: %s\n", strerror(errno));
			
				ret = FAILURE;
				goto send_ext;
			}
			if(rd == 0)		//file ended
				break;
		}
		
		tmp = waitpid(chld_pid, &stat, WNOHANG);
		if(tmp == chld_pid)
		{
			fprintf(stderr, "Child exited earlier than the copy ended with exit code %d.\n", WEXITSTATUS(stat));
			chld_pid = 0;

			ret = FAILURE;
			goto send_ext;
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

	stat = kill(chld_pid, SIGINT);
	if(stat < 0)
	{
		fprintf(stderr, "Can't send signal to child process: %s\n", strerror(errno));
			
		ret = FAILURE;
		goto send_ext;
	}

	#ifdef __LOG
		printf("Parent:" "\t" "time = %li:" "\t" "Signal SIGINT was sent to a child.\n", time(NULL));
	#endif

send_ext:
	
	if(fSrc)
	{
		close(fSrc);
		fSrc = 0;
	}

	if(fDst)
	{
		close(fDst);
		fDst = 0;
	}

	if(chld_pid)
	{
		wait(&stat);
		
		ret = WEXITSTATUS(stat);
		fprintf(stdout, "Child exited with return code %d.\n", ret);

		chld_pid = 0;
	}

	return ret;
}

int recieve()
{
	sigset_t mask, maskint;
	int stat = 0, ret = 0, tmp = 0;

	close(fSrc);

#ifdef __DBG
	FILE* fPid = fopen("sigcp2.pid", "w");
	fprintf(fPid, "%d", getpid());
	fclose(fPid);
	fPid = 0;
#endif

	struct sigaction sa, sa_old;
	memset(&sa, 0, sizeof(sa));
	
	sa.sa_handler = recieve_usr_handler;
	sa.sa_mask = mask;
	
	stat = sigaction(SIGUSR1, &sa, &sa_old);
	if(stat < 0)
	{
		fprintf(stderr, "Can't use sigaction: %s\n", strerror(errno));

		ret = FAILURE;
		goto recieve_ext;
	}

	stat = sigaction(SIGUSR2, &sa, &sa_old);
	if(stat < 0)
	{
		fprintf(stderr, "Can't use sigaction: %s\n", strerror(errno));

		ret = FAILURE;
		goto recieve_ext;
	}

	sa.sa_handler = inthandler;

	stat = sigaction(SIGINT, &sa, &sa_old);
	if(stat < 0)
	{
		fprintf(stderr, "Can't use sigaction: %s\n", strerror(errno));

		ret = FAILURE;
		goto recieve_ext;
	}

	memset(buf, 0, PART_SIZE);

	int first = true;

	stat = sigfillset(&mask);
	if(stat < 0)
	{
		fprintf(stderr, "Can't modify mask.\n");

		ret = FAILURE;
		goto recieve_ext;
	}

	stat = sigdelset(&mask, SIGUSR1);
	if(stat < 0)
	{
		fprintf(stderr, "Can't modify mask.\n");

		ret = FAILURE;
		goto recieve_ext;
	}

	stat = sigdelset(&mask, SIGUSR2);
	if(stat < 0)
	{
		fprintf(stderr, "Can't modify mask.\n");

		ret = FAILURE;
		goto recieve_ext;
	}

	stat = sigdelset(&mask, SIGINT);
	if(stat < 0)
	{
		fprintf(stderr, "Can't modify mask.\n");

		ret = FAILURE;
		goto recieve_ext;
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
				goto recieve_ext;
			}
			
			memset(buf, 0, PART_SIZE);
		}
		
		stat = kill(par_pid, SIGUSR1);
		if(stat < 0)
		{
			fprintf(stderr, "Can't send signal to parent process: %s\n", strerror(errno));
				
			ret = FAILURE;
			goto recieve_ext;
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

recieve_ext:

	if(fDst > 0)
	{
		close(fDst);
		fDst = 0;
	}
	
	if(fSrc > 0)
	{
		close(fSrc);
		fSrc = 0;
	}

	if(ret != 0)
	{
		kill(par_pid, SIGUSR2);


		#ifdef __LOG
			printf("Child:" "\t" "time = %li:" "\t" "Signal SIGUSR2 was sent to parent.\n", time(NULL));
		#endif
	}

#ifdef __NORES
	remove(dstPath);
#endif

	return ret;
}

void send_usr_handler(int signal)
{
	if(signal == SIGUSR2)
	{
	#ifdef __LOG
		printf("Parent:" "\t" "time = %li:" "\t" "Signal SIGUSR2 detected.\n", time(NULL));
	#endif

		bytecount = -1;
		return;
	}

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
		stat = kill(chld_pid, SIGUSR2);
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
	
	stat = kill(chld_pid, SIGUSR1);
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

void recieve_usr_handler(int signal)
{
#ifdef __LOG
	printf("Child:" "\t" "time = %li:" "\t" "Signal SIGUSR%d detected.\n", time(NULL), ((signal == SIGUSR2) + 1));
#endif
	char tmp = (signal == SIGUSR2);
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
