//Made by 575 group's student Shiyanov Vadim.

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define PART_SIZE 16384
#define BAD_INPUT 1
#define FAILURE 2
//#define _DBG			//delete in final version

void usage(char*);
int ml_copy(char*, char*);
int do_copy(int, int, char*, char*);

int main (int argc, char** argv)
{
	if(argc != 3) {
		fprintf(stderr, "Invalid arguments\n");
		usage(argv[0]);
		return BAD_INPUT;
	}
	
	return ml_copy(argv[1], argv[2]);
}

void usage(char* prog_name)
{
	fprintf(stderr, "Usage:\n"
		"\t%s SRC_FILE DST_FILE\n", prog_name);
}

int ml_copy(char* src, char* dst)
{
#ifdef _DBG
	int t1 = time(0);
#endif

	int fSrc = -1, fDst = -1;
	char buf[PART_SIZE];
	int ret;

	fSrc = open(src, O_RDONLY);
	if (fSrc < 0) {
		fprintf(stderr, "Can't open file: %s: %s\n",
			src,
			strerror(errno)
			);
		ret = BAD_INPUT;
		goto ext;
	}

	fDst = open(dst, O_CREAT | O_EXCL | O_WRONLY, 0666);
	if (fDst < 0) {
		fprintf(stderr, "Can't open file: %s: %s\n",
			dst,
			strerror(errno)
			);
		ret = FAILURE;
		goto ext;
	}
	
	ret = do_copy(fSrc, fDst, src, dst);

ext:
	if (fSrc >= 0)
		close(fSrc);
	if (fDst >= 0)
		close(fDst);

#ifdef _DBG
	int t2 = time(0);
	fprintf(stderr, "------------------------------------------\n"
			"WARNING: if you see this, than it is either it is a debugging version, or I" 
			" haven't undefined __DEBUG.\n");
	fprintf(stdout, "%d\n", t2 - t1);
	fprintf(stderr, "------------------------------------------\n");
	remove(dst);		//for correct debugging
#endif

	return ret;
}

int do_copy(int fSrc, int fDst, char* src, char* dst)
{
	static char buf[PART_SIZE];

	int rdCheck = 1;
	int wrCheck = 0;
	while(rdCheck != 0) {
		rdCheck = read(fSrc, buf, PART_SIZE);
		if (rdCheck < 0) {
			fprintf(stderr, "Can't write in file: %s: %s\n",
				dst,
				strerror(errno)
				);
			return FAILURE;
		}

		wrCheck = write(fDst, buf, rdCheck);
		if (wrCheck < 0) {
			fprintf(stderr, "Can't read from file: %s: %s\n",
				src,
				strerror(errno)
				);
			return FAILURE;
		}
	}
	
	return 0;
}
