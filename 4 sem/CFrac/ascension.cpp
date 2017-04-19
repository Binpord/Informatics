#include "std.h"
#include "newCFrac.h"

void usage(char*);
int asc(int, int, int);

int main(int argc, char** argv)
{
	if(argc != 4)
	{
		usage(argv[0]);
		return 1;
	}

	return asc(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
}

void usage(char* progname)
{
	std::cout << "Usage:" << std::endl;
	std::cout << "\t" << progname << " n m k" << std::endl;
}

int asc(int n, int m, int k)
{
	CFrac pos(0, 0, 0),todo(1, 0, 0), dnm(0, 1, n), dnm2(0, 1, k);

	for(int i = 0; i < m - 1; i++)
	{
		// проходим 1/n оставшегося расстояния
		pos = pos + (todo * dnm);
		todo = todo - (todo * dnm);

		// падаем на 1/k пройденного расстояния
		todo = todo + (pos * dnm2);
		pos = pos - (pos * dnm2);
	}

	pos = pos + (todo * dnm);
	todo = todo - (todo * dnm);
	// на выходе имеем нынешнюю позицию, когда до вершины остался метр
	CFrac meters(1, 0, 0);
	meters = meters / todo;
	meters.print();
	return 0;
}
