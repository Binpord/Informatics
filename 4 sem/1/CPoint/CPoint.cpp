#include "CPoint.h"

CPoint::CPoint()
{
	x = 0;
	y = 0;
}

void CPoint::getVal()
{
	printf("Please, write coordinates like: x, y\n");
	scanf("%d, %d", &x, &y);
}

void CPoint::moveOn(int x1, int y1)
{
	x += x1;
	y += y1;
}

int CPoint::diffX(CPoint a)
{
	return a.x - x;
}

int CPoint::diffY(CPoint a)
{
	return a.y - y;
}


void CPoint::print()
{
	printf("(%d, %d)\n", x, y);
}
