#include "CFrac.h"

CFrac::CFrac()
{
	un = 0;
	num = 0;
	denom = 0;
}

CFrac::CFrac(int in_un, int in_num, int in_denom)
{
	if(in_denom == 0 && in_num != 0)
	{
		fprintf(stderr, "Error, no division by 0\n");
		return;
	}

	un = in_un;
	num = in_num;
	denom = in_denom;

	if(num > denom)
	{
		un += num / denom;
		num %= denom;
	}
	
	if(num == 0)
		denom = 0;
	else
		simplify(&num, &denom);
}

void CFrac::setVal(int in_un, int in_num, int in_denom)
{
	if(in_denom == 0 && in_num != 0)
	{
		fprintf(stderr, "Error, no division by 0\n");
		return;
	}

	un = in_un;
	num = in_num;
	denom = in_denom;

	if(num > denom)
	{
		un += num / denom;
		num %= denom;
	}
	
	if(num == 0)
		denom = 0;
	else
		simplify(&num, &denom);
}

int CFrac::getUn()
{
	return un;
}

int CFrac::getNum()
{
	return num;
}

int CFrac::getDenom()
{
	return denom;
}

CFrac CFrac::operator+(const CFrac& comp)
{
	int res_un = 0, res_num = 0, res_denom = 0;

	res_un = un + comp.un;
	res_denom = denom * comp.denom;
	res_num = num * comp.denom + comp.num * denom;
	if(res_num > res_denom)
	{
		res_un += res_num / res_denom;
		res_num %= res_denom;
	}

	simplify(&res_num, &res_denom);

	CFrac res(res_un, res_num, res_denom);

	return res;
}

CFrac& CFrac::operator++(int)
{
	un++;
	return *this;
}

CFrac& CFrac::operator!()
{
	if(num == 0 && denom != 0)
	{
		fprintf(stderr, "Error, no division by 0\n");
		return *this;
	}

	int tmp = num;
	num = denom;
	denom = num;
	//if(num > denom) - as the frac was relevant before, this will be used
	//every time
	//{
	un += num/denom;
	num %= denom;
	//}
	simplify(&num, &denom);
	return *this;
}

void CFrac::print()
{
	if(num == 0)
		printf("%d\n", un);
	else
		printf("%d (%d / %d)\n", un, num, denom);
}

void CFrac::simplify(int* num1, int* num2)
{
	//finding GCD for numbers
	int tmpa = *num1, tmpb = *num2, tmpc = 0;
	while(tmpb)
	{
		tmpc = tmpa % tmpb;
		tmpa = tmpb;
		tmpb = tmpc;
	}
	//GCD is in tmpa

	*num1 /= tmpa;
	*num2 /= tmpa;
}

int CFrac::operator<(const CFrac& comp)
{
	if(un < comp.un)
		return true;
	else if(un > comp.un)
		return false;

	//un = cmp.un
	int base_num = num * comp.denom;
	int comp_num = comp.num * denom;

	if(base_num < comp_num)
		return true;
	else 
		return false;
}

int CFrac::operator==(const CFrac& comp)
{
	if(un == comp.un && num == comp.num && denom == comp.denom)
		return true;
	else
		return false;
}

int CFrac::operator>(const CFrac& comp)
{
	if(*this < comp || *this == comp)
		return false;
	else
		return true;
}
