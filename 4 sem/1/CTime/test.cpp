#include "CTime.h"

int main()
{
	CTime a, b, c;
	a.print();
	
	a.setTime(13, 20);
	b.setTime(13, 40);
	c = a.diff(b);
	c.print();

	return 0;
}
