#include "CRect.h"

int main()
{
	CPoint hl, lr;
	hl.getVal();
	lr.getVal();
	CRect rect1;
	rect1.setPoints(hl, lr);
	hl.getVal();
	lr.getVal();
	CRect rect2;
	rect2.setPoints(hl, lr);
	CRect* rect3 = rect1.crossRect(rect2);
	rect3->print();
	return 0;
}
