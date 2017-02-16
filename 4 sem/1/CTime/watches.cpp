#include "CTime.h"

void usage(char*);
int start_count(int, int);

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		fprintf(stderr, "Incorrect input.\n");
		usage(argv[0]);
		return BAD_IN;
	}

	int t1 = 0, t2 = 0;
	sscanf(argv[1], "%d", &t1);
	sscanf(argv[2], "%d", &t2);	//know, about unsafty of sprintf
					//but here is only 2 ints

	return start_count(t1, t2);
}

void usage(char* progname)
{
	fprintf(stderr, "Usage:\n"
				"\t%s t1 t2\n", progname);
}

int start_count(int t1, int t2)
{
	//Shlyacsic is exhibiting his watches every 30 min, while 
	//Byacsic only once an hour.
	CTime shl, bcs, shl_shift, bcs_shift, ha_shift;

	if(30 + t1 > 60)
		shl_shift.setTime(1, 30 + t1);
	else
		shl_shift.setTime(0, 30 + t2);

	bcs_shift.setTime(0, 30 - t2);

	ha_shift.setTime(0, 30);
	
	int i = 0;
	for(; bcs.isLater(shl) || !i; i++)
	{
		shl = shl.add(shl_shift);
		shl.print();
		if(i % 2)
		{
			bcs = bcs.add(ha_shift);
			bcs.print();
		}
		else
		{
			bcs = bcs.add(bcs_shift);
			bcs.print();
		}
	}

	int days = 0, hours = 0, mins = 0;

	if(i % 2)
	{
		mins = 30;
		i--;
	}

	hours = i / 2;
	days = hours / 24;
	hours %= 24;

	printf("Their watches will sync after %d days, %d hours and %d minutes.\n", 		days, hours, mins);

	return 0;
}
