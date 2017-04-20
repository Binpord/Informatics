#include "CLine.h"

CLine::CLine()
{
	// start initialized by his own c-tor
	length = 0;
	ishor = false;
}

void CLine::SetLine(const CPoint& s, const CPoint& e)
{
	length = s.diffX(e);		// length = e - s
	if(length != 0)
	{
		if(s.diffY(e) != 0)		// assuming this to be incorrect input
		{				// as only horizontal or vertical support
			start.setPoint(0, 0);
			length = 0;
			ishor = false;
			return;
		}

		ishor = true;
		if(length < 0)
		{
			length *= -1;
			start = e;
		}
		else
			start = s;

		return;
	}
	
	length = s.diffY(e);
	ishor = false;
	
	if(length < 0)
	{
		length *= -1;
		start = e;
	}
	else
		start = s;
	return;
}

void CLine::GetLine()
{
	int tmp;
	start.getVal();
	std::cout << "Please, enter length and ishor (0 or 1) like length, ishor" << std::endl;
	//scanf("%d, %b", &length, &ishor);
	//std::cin >> length >> tmp >> tmp >> ishor >> tmp;			// ignore ", "
	scanf("%d, %d", &length, &tmp);
	ishor = tmp;
}

CLine::CLine(const CPoint& s, const CPoint& e)
{
	this->SetLine(s, e);
}

CPoint CLine::GetStart() const
{
	return start;
}

int CLine::GetLength() const
{
	return length;
}

bool CLine::IsHor() const
{
	return ishor;
}

bool CLine::IsPoint() const
{
	if(length != 0)
		return false;

	return true;
}

int CLine::operator=(const CLine& eq)
{
	start = eq.start;
	length = eq.length;
	ishor = eq.ishor;
	return 1;
}

void CLine::print() const
{
	if(length == 0)
	{
		start.print();
		return;
	}
	CPoint tmp;
	tmp.setPoint(tmp.diffX(this->start), tmp.diffY(this->start));
	if(ishor == false)
		tmp.moveOn(0, length);
	else
		tmp.moveOn(length, 0);

	start.endllessprint();
	std::cout << " -> ";
	tmp.print();
}

//===============================================================================================
// everything beneath this line is for the public Intercept function

int CLine::dot_interception(const CLine& hor, const CLine& vert, CLine* storage /* = NULL */) const
{
	int xrange = vert.start.diffX(hor.start), yrange = vert.start.diffY(hor.start);		// xrange = hor.start - vert.start, yrange = hor.start - vert.start
												// start = low left

	if(xrange > 0)			// no interception as hor.start (left) located right from the vert.start
		return 0;		// 0 is for no interception
	else 
	{
		xrange *= -1;
		if(xrange > hor.length)		// no interception, as vert.start is too far right 
			return 0;
		else				// dot-interception if yrange is normal
		{
			if(yrange < 0 || yrange > vert.length)		// no interception as hor.start (low left) is rather lower
				return 0;				// than low left of vert or too high
			else
			{
				if(storage != NULL)
				{
					CPoint res_start;
					res_start.moveOn(res_start.diffX(vert.start), res_start.diffY(hor.start));
					storage->start = res_start;
					storage->length = 0;
				}
				return 1;				// 1 is for dot-interception
			}
		}
	}			

	return -1;			// never here
}

int CLine::make_line_interception(const CLine& left, const CLine& right, int dist, CLine* storage /* = NULL */) const
{
	if(left.length >= dist + right.length)
	{
		if(storage != NULL)
			*storage = right;
		return 3;			// 3 is for line-interception
	}
	else
	{
		if(storage != NULL)
		{
			storage->start = right.start;
			storage->length = left.length - dist;
			storage->ishor = left.ishor;
		}
		return 3;
	}
}

int CLine::line_interception(const CLine& comp1, const CLine& comp2, int dist, CLine* storage /* = NULL */) const
{
	/*
	 * xrange = start - comp, if xrange < 0 (start located left from comp) and
	 * (-xrange) > length or if xrange > 0 (comp located left from start) and
	 * xrange < comp.length, than no interception
	 */
	if((dist > 0 && dist > comp2.length) || (dist < 0 && (-1) * dist > comp1.length))
		return 2;
	else if(dist > 0)		// line-interception, comp located left from start
		return make_line_interception(comp2, comp1, dist, storage);
	else
		return make_line_interception(comp1, comp2, (-1) * dist, storage);
}

// mind that storage will be used in output
int CLine::Intercept(const CLine& comp, CLine* storage /* = NULL */) const
{
	int xrange = comp.start.diffX(start), yrange = comp.start.diffY(start);		// xrange = start - comp, yrange = start - comp
											// start = low left
/*
 * Moved to the dot-interception function
	if(ishor == true && comp.ishor == false)			// lines are perpendicular, this - horizontal, comp - vertical
	{
		if(xrange > 0)			// no interception as this.start (left) located right from the comp.start
			return 0;		// 0 is for no interception
		else 
		{
			xrange *= -1;
			if(xrange > length)		// no interception, as comp.start is right too far
				return 0;
			else				// dot-interception if yrange is normal
			{
				if(yrange < 0 || yrange > comp.length)		// no interception as this.start (low left of horizontal this) is rather lower
					return 0;				// than low left of comp or too high
				else
				{
					CPoint res_start;
					res_start.setVal(res_start.diffX(comp.start), res_start.diffY(start));
					storage->start = res_start;
					storage->length = 0;
					return 1;				// 1 is for dot-interception
				}
			}
		}			
	}
*/
	// dot-interception
	if(ishor == true && comp.ishor == false)
		return dot_interception(*this, comp, storage);
	else if(ishor == false && comp.ishor == true)
		return dot_interception(comp, *this, storage);
	// line-interception
	// ishor == comp.ishor
	else if(ishor == true /* && comp.ishor == true*/)
	{
		if(yrange != 0)
			return 2;	// 2 is for parallel, but no interception
	/*
	 * Moved to line-interception function
		else
		{
			/\*
			 * xrange = start - comp, if xrange < 0 (start located left from comp) and 
			 * (-xrange) < length or if xrange > 0 (comp located left from start) and
		 	 * xrange < comp.length, than no interception
			 *\/
		 	if((xrange > 0 && xrange < comp.length) || (xrange < 0 && xrange > length))
				return 2;
			else if(xrange > 0)		// line-interception, comp located left from start
				return line-interception(comp, *this, xrange, storage);
			else
				return line-interception(*this, comp, (-1) * xrange, storage);
		}
	*/
		else
			return line_interception(*this, comp, xrange, storage);
	}
	else if(ishor == false /* && comp.ishor == false */)
	{
		if(xrange != 0)
			return 2;
		else
			return line_interception(*this, comp, yrange, storage);
	}

	// never here
	return -1;
}
