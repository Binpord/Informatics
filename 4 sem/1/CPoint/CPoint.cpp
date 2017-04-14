#include "CPoint.h"

CPoint::CPoint()
{
	x = 0;
	y = 0;
}

CPoint::CPoint(int in_x, int in_y)
{
	x = in_x, y = in_y;
}

int CPoint::GetX() const
{
	return x;
}

int CPoint::GetY() const
{
	return y;
}

void CPoint::getVal()
{
	printf("Please, write coordinates like: x, y\n");
	scanf("%d, %d", &x, &y);
}

void CPoint::setPoint(int in_x, int in_y)
{
	x = in_x, y = in_y;
}

void CPoint::moveOn(int x1, int y1)
{
	x += x1;
	y += y1;
}

int CPoint::diffX(const CPoint& a) const
{
	return a.x - x;
}

int CPoint::diffY(const CPoint& a) const
{
	return a.y - y;
}

void CPoint::print() const
{
	printf("(%d, %d)\n", x, y);
}

void CPoint::endllessprint() const
{
	std::cout << "(" << x << ", " << y << ")";
}

int CPoint::operator=(CPoint comp)
{
	x = comp.x, y = comp.y;
	return true;
}
