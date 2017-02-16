#include "std.h"
#include "CFrac.h"

int main()
{
	CFrac a(0, 6, 13), b(0, 7, 9), c;
	c = a + b;
	a.print();
	b.print();
	c.print();

	a.setVal(0, 7, 20);
	b.setVal(0, 3, 20);
	c = a + b;
	c.print();

	a.setVal(1, 20, 4);
	a.print();
	b.setVal(3, 0, 0);
	if(a > b)
		printf("correct\n");

	return 0;
}
