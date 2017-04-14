#include "CLine.h"

int main()
{
	CLine a, b, c;
	a.GetLine();
	b.GetLine();
	a.Intercept(b, &c);
	c.print();
}
