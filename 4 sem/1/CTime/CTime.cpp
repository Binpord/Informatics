#include "CTime.h"

CTime::CTime()
{
	h = 0;
	min = 0;
}

void CTime::print()
{
	printf("%d:%d\n", h, min);
}

void CTime::setTime(int _h, int _min)
{
	if(_h < 24 && _h >= 0)
		h = _h;
	else
		printf("%d is incorrect amount of hours\n", _h);

	if(_min < 60 && _min >= 0)
		min = _min;
	else
		printf("%d is incorrect amount of minutes\n", _min);
}

int CTime::isLater(CTime base)
{
	if(base.h > h)
		return -1;
	else if(base.h < h)
		return 1;
	else if(base.min > min)		//base.h = h
		return -1;
	else if(base.min < min)
		return 1;
	
	return 0;
}

CTime CTime::div(CTime base, CTime subtrh)
{
	CTime res;
	int res_h = base.h - subtrh.h, res_min = base.min - subtrh.min;
	
	if(res_min < 0)
	{
		res_min += 60;
		res_h--;
	}
	
	if(res_h < 0)
	{
		res_h += 24;
	}
	// часы и минуты корректны

	res.setTime(res_h, res_min);
	return res;
}

CTime CTime::diff(CTime cmp)
{
	CTime res;
	int isLt = isLater(cmp);
	if(isLt == 1)
		return div(*this, cmp);
	else if(isLt == -1)
		return div(cmp, *this);
	else
		return res;
}

CTime CTime::add(CTime smnd)
{
	CTime res;
	int res_h = h + smnd.h, res_min = min + smnd.min;

	if(res_min >= 60)
	{
		res_min -= 60;
		res_h++;
	}
	else if(res_min < 0)
	{
		res_min += 60;
		res_h--;
	}
	if(res_h >= 24)
		res_h -= 24;
	else if(res_h < 0)
		res_h += 24;

	res.setTime(res_h, res_min);
	return res;
}
