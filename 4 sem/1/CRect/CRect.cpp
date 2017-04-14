#include "CRect.h"

CRect::CRect()
{
	//every part of info in the class is another class => construction
	//achieved by using their constructors.
}

CRect::~CRect()
{}

void CRect::print() const
{
	printf("high left: ");
	hl.print();
	printf("low right: ");
	lr.print();
}

void CRect::setPoints(const CPoint& in_hl, const CPoint& in_lr)
{
	hl.moveOn(hl.diffX(in_hl), hl.diffY(in_hl));
	lr.moveOn(lr.diffX(in_lr), lr.diffY(in_lr));
}

CPoint CRect::getLeft() const
{
	return hl;
}

CPoint CRect::getRight() const
{
	return lr;
}

CPoint CRect::getCenter() const
{
	CPoint res;
	res.moveOn(res.diffX(hl), res.diffY(hl));
	res.moveOn(res.diffX(lr) / 2, res.diffY(lr) / 2);
	return res;
}

void CRect::moveToPoint(CPoint a)
{
	CPoint c = this->getCenter();
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
	CPoint c = this->getCenter();
	CPoint new_hl, new_lr;
	new_hl.moveOn(new_hl.diffX(c), new_hl.diffY(c));
	new_lr.moveOn(new_lr.diffX(c), new_lr.diffY(c));
	new_hl.moveOn(c.diffY(hl), c.diffX(hl));
	new_lr.moveOn(c.diffY(lr), c.diffX(lr));

	lr = new_lr;
	hl = new_hl;
}

bool CRect::IsIn(const CRect& comp) const
{
	if( 		   (hl.diffX(comp.hl) < 0 && hl.diffX(comp.lr) > 0)		// hl is between comp.hl and comp.lr
			&& (hl.diffY(comp.lr) < 0 && hl.diffY(comp.hl) > 0)
			&& (lr.diffX(comp.hl) < 0 && lr.diffX(comp.lr) > 0)		// lr is between comp.hl and comp.lr
			&& (lr.diffY(comp.hl) < 0 && lr.diffY(comp.lr) > 0)
	  )
		return true;
	else
		return false;
}

CRect* CRect::crossRect(const CRect& comp) const
{
	CRect *Res = new CRect;
	CPoint pntTmp;
	CLine lnIntTmp;
	// creating lines for this rect
	pntTmp.moveOn(pntTmp.diffX(hl), pntTmp.diffY(lr));
	CLine thisLeft(hl, pntTmp);
	pntTmp.moveOn(pntTmp.diffX(lr), pntTmp.diffY(hl));
	CLine thisRight(lr, pntTmp);
	pntTmp.moveOn(pntTmp.diffX(hl), pntTmp.diffY(lr));
	CLine thisLow(lr, pntTmp); 
	pntTmp.moveOn(pntTmp.diffX(lr), pntTmp.diffY(hl));
	CLine thisHigh(hl, pntTmp);
	// creating lines for comp rect
	pntTmp.moveOn(pntTmp.diffX(comp.hl), pntTmp.diffY(comp.lr));
	CLine compLeft(comp.hl, pntTmp);
	pntTmp.moveOn(pntTmp.diffX(comp.lr), pntTmp.diffY(comp.hl));
	CLine compRight(comp.lr, pntTmp);
	pntTmp.moveOn(pntTmp.diffX(comp.hl), pntTmp.diffY(comp.lr));
	CLine compLow(comp.lr, pntTmp);
	pntTmp.moveOn(pntTmp.diffX(comp.lr), pntTmp.diffY(comp.hl));
	CLine compHigh(comp.hl, pntTmp);
	// intercepting lines and parsing inteceptions
	// if no interceptions => rects either hase no cross or one of them is located in the other one
	if(thisLeft.Intercept(compLow) != 0)	// there is interception
	{					// only dot-intercepting is available
		/*
		 *			---------------------------------
		 *			|				|
		 *			|				|
		 *			|				|
		 *			|	| <- this		|
		 *			|	|			|
		 *			|	|		comp ->	|
		 *			---------------------------------
		 *				|
		 *				|
		 *				-------
		 */
		// finding Res->hl
		if(thisLeft.Intercept(compHigh, &lnIntTmp) != 0)	// is interception, means lnIntTmp contains Res->hl
			Res->hl = lnIntTmp.GetStart();
		else			// no interception means that Res->hl is this->hl
			Res->hl = hl;

		// finding Res->lr
		if(thisRight.Intercept(compLow, &lnIntTmp) != 0)	// is interception, means that Res->lr is in lnIntTmp
			Res->lr = lnIntTmp.GetStart();
		else			// no interceotion means that Res->lr is comp.lr
			Res->lr = comp.lr;

		return Res;
	}

	if(thisLeft.Intercept(compHigh, &lnIntTmp) != 0)		// if there is interception
	{				// however no interception between thisLeft and compLow
		/*
		 *				------
		 *				| <- this
		 *				|
		 *			-----------------------------------------
		 *			|	|				|
		 *			|	|				|
		 *			|	------				|
		 *			|	no intetception with		|
		 *			|	low this time		comp ->	|
		 *			-----------------------------------------
		 */
		Res->hl = lnIntTmp.GetStart();		//if there is interception, than Res->hl is in lnIntTmp

		;
		if(thisLow.Intercept(compRight, &lnIntTmp) != 0)	// is interception, means Res->lr is in lnIntTmp
			Res->lr = lnIntTmp.GetStart();
		else
			Res->lr = lr;

		return Res;
	}

	if(thisRight.Intercept(compLow, &lnIntTmp) != 0)
	{
		/*
		 *			---------------------------------
		 *			|			comp ->	|
		 *			|				|
		 *			|				|
		 *		--------|------------------------	|
		 *		|	|		this ->	|	|
		 *		|	|			|	|
		 *		|	---------------------------------
		 *		|				|
		 *		|				|
		 *		---------------------------------
		 */
		Res->lr = lnIntTmp.GetStart();

		thisHigh.Intercept(compLeft, &lnIntTmp);
		Res->hl = lnIntTmp.GetStart();

		return Res;
	}

	if(thisRight.Intercept(compHigh, &lnIntTmp) != 0)
	{
		/*
		 *		---------------------------------
		 *		|				|
		 *		|				|
		 *		|				|
		 *	--------|----------------		|
		 *	|	|	this ->	|		|
		 *	|	|		|	comp ->	|
		 *	|	---------------------------------
		 *	|			|
		 *	|			|
		 *	-------------------------
		 */
		Res->lr = lnIntTmp.GetStart();

		thisHigh.Intercept(compLow, &lnIntTmp);
		Res->hl = lnIntTmp.GetStart();

		return Res;
	}

	if(compLeft.Intercept(thisLow, &lnIntTmp) != 0)
	{
		/*
		 *			---------------------------------
		 *			|				|
		 *			|				|
		 *			|				|
		 *			|	|  	     |		|
		 *			|	|	     |		|
		 *			|	|	     |	this ->	|
		 *			---------------------------------
		 *		    lnIntTmp ->	|	     |<- else thisRight.Intercept(compHigh)
		 *				|<- comp     |
		 *				--------------
		 */
		thisLow.Intercept(compRight, &lnIntTmp);
		Res->lr = lnIntTmp.GetStart();

		if(compLeft.Intercept(thisHigh, &lnIntTmp) != 0)
			Res->hl = lnIntTmp.GetStart();
		else
			Res->hl = comp.hl;

		return Res;
	}

	if(compLeft.Intercept(thisHigh, &lnIntTmp) != 0)
	{
		/*
		 * 				-----------------
		 * 		     lnIntTmp->	|		|<- else thisRigh.Intercept(compLow)
		 *			---------------------------------
		 *			|	|		|	|
		 *			|	|<-comp		|	|
		 *			|				|
		 *			|				|
		 *			|				|
		 *			|				|
		 *			|			this ->	|
		 *			---------------------------------
		 */
		Res->hl = lnIntTmp.GetStart();

		if(compLeft.Intercept(thisLow, &lnIntTmp) != 0)
			Res->lr = lnIntTmp.GetStart();
		else
			Res->lr = comp.lr;

		return Res;
	}

	// no line interceptions => either no interception or one rect is located in another
	if(this->IsIn(comp))
	{
		*Res = *this;
		return Res;
	}
	else if(comp.IsIn(*this))
	{
		*Res = comp;
		return Res;
	}
	else
	{
		delete[] Res;
		return NULL;
	}
}
