#include "CRect.h"

CRect::CRect()
{
	//every part of info in the class is another class => construction
	//achieved by using their constructors.
}

CRect::~CRect()
{}

void CRect::print()
{
	printf("high left: ");
	hl.print();
	printf("low right: ");
	lr.print();
}

void CRect::setPoints(CPoint in_hl, CPoint in_lr)
{
	hl.moveOn(hl.diffX(in_hl), hl.diffY(in_hl));
	lr.moveOn(lr.diffX(in_lr), lr.diffY(in_lr));
}

CPoint CRect::getLeft()
{
	return hl;
}

CPoint CRect::getRight()
{
	return lr;
}

void CRect::crossRect(CRect a)
{
	if(
}
