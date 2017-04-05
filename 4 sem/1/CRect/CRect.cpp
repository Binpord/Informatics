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

CPoint CRect::getCenter()
{
	CPoint res;
	res.moveOn(res.diffX(hl), res.diffY(hl));
	res.moveOn(res.diffX(lr) / 2, res.diffY(lr) / 2);
	return res;
}

void CRect::moveToPoint(CPoint a)
{
	CPoint c = this.getCenter();
	hl.moveOn(c.diffX(a), c.diffY(a));
	lr.moveOn(c.diffX(a), c.diffY(a));
}

void CRect::mirrorV()
{
	CPoint o;	//origin achieved by the constructor
	hl.moveOn(hl.diffX(o) * 2, 0);
	lr.moveOn(lr.diffX(o) * 2, 0);
}

void CRect::mirrorH()
{
	CPoint o;
	hl.moveOn(0, hl.diffY(o) * 2);
	lr.moveOn(0, lr.diffY(o) * 2);
}

void CRect::rotate90()
{
	CPoint c = this.getCenter();
	CPoint new_hl, new_lr;
	new_hl.moveOn(new_hl.diffX(c), new_hl.diffY(c));
	new_lr.moveOn(new_lr.diffX(c), new_lr.diffY(c));
	new_hl.moveOn(c.diffY(hl), c.diffX(hl));
	new_lr.moveOn(c.diffY(lr), c.diffX(lr));

	lr = new_lr;
	hl = new_hl;
}

CRect* CRect::crossRect(CRect a)
{
}
