#include "newCFrac.h"

int CFrac::simplify(int* num1, int* num2)
{
	if(*num1 == 0 || *num2 == 0)
		return 1;

	// finding GCD for numbers
	int tmpa = *num1, tmpb = *num2, tmpc = 0;
	while(tmpb)
	{
		tmpc = tmpa % tmpb;
		tmpa = tmpb;
		tmpb = tmpc;
	}
	// GCD is in tmpa

	*num1 /= tmpa;
	*num2 /= tmpa;
	return 0;
}

// best fuction ever (normalizing frac, calling it after manual changing
// of info guarantee you that it will have its uniq form)
// P.S. mind that if input is incorrect, the output will be zero initialized frac
int CFrac::normalize()
{
	if(num < 0 || denom < 0)
	{
		num = 0, denom = 0;
		return 1;
	}
	else if(denom == 0)
	{
		if(num != 0)
		{
			num = 0;
			return 1;
		}
		else
			return 0;
	}

	simplify(&num, &denom);
	return 0;
}

CFrac::CFrac()
{
	num = 0, denom = 0;
}

CFrac::CFrac(int in_un, int in_num, int in_denom)
{
	if(in_denom == 0 && in_un != 0)
	{
		if(in_num != 0)
		{
			num = 0, denom = 0;
			return;
		}
		
		num = in_un, denom = 1;
		return;
	}
	num = in_un * in_denom + in_num, denom = in_denom;
	normalize();		// no bug handling((
}

CFrac::CFrac(int in_num, int in_denom)
{
	num = in_num, denom = in_denom;
	normalize();
}

int CFrac::getNum()
{
	return num;
}

int CFrac::getDenom()
{
	return denom;
}

void CFrac::setVal(int in_un, int in_num, int in_denom)
{
	CFrac(in_un, in_num, in_denom);
}

void CFrac::setVal(int in_num, int in_denom)
{
	num = in_num, denom = in_denom;
	normalize();
}

CFrac CFrac::operator-(const CFrac& sub)
{
	if(sub.num == 0 && sub.denom == 0)
	{
		return *this;
	}
	// no handling if(num == 0) as there is no support of negative numbers
	int res_num = num * sub.denom - sub.num * denom, res_denom = denom * sub.denom;
	if(res_num == 0)
		res_denom = 0;
	CFrac res(res_num, res_denom);
	res.normalize();
	return res;
}

CFrac CFrac::operator+(const CFrac& add)
{
	if(num == 0 && denom == 0)
		return add;
	else if(add.num == 0 && add.denom == 0)
		return *this;
	int res_num = num * add.denom + add.num * denom, res_denom = denom * add.denom;
	CFrac res(res_num, res_denom);
	res.normalize();
	return res;
}

CFrac CFrac::operator+(int add)
{
	if(denom == 0 && num == 0)
	{
		int res_num = add, res_denom = 1;
		CFrac res(res_num, res_denom);
		return res;
	}
	int res_num = num + add * denom;
	CFrac res(res_num, denom);
	return res;
}

CFrac& CFrac::operator!()
{
	int tmp = num;
	denom = num, num = tmp;		// no need to normalize if it is normal by now
	return *this;
}

int CFrac::operator=(const CFrac& comp)
{
	num = comp.num, denom = comp.denom;
	return true;
}

CFrac CFrac::operator*(const CFrac& mul)
{
	int res_num = num * mul.num, res_denom = denom * mul.denom;
	CFrac res(res_num, res_denom);
	res.normalize();
	return res;
}

CFrac CFrac::operator/(const CFrac& div)
{
	int res_num = num * div.denom, res_denom = denom * div.num;
	CFrac res(res_num, res_denom);
	res.normalize();
	return res;
}

int CFrac::operator<(const CFrac& comp)
{
	return (comp.num * denom - num * comp.denom > 0);
}

int CFrac::operator>(const CFrac& comp)
{
	return (num * comp.denom - comp.num * denom > 0);
}
int CFrac::operator==(const CFrac& comp)
{
	return (num == comp.num && denom == comp.denom);	// works this way, assuming that both fracs normalized
}

void CFrac::print()
{
	if(num == 0 && denom == 0)
	{
		std::cout << "0" << std::endl;
		return;
	}
	else if(denom == 1)			// equivalent condition to (num % denom == 0) due to normalization
	{					// but easier to check
		std::cout << num << std::endl;
		return;
	}
	else if(num < denom)
	{
		std::cout << "(" << num << "/" << denom << ")" << std::endl;
		return;
	}
	std::cout << num / denom << "(" << num % denom << "/" << denom << ")" << std::endl;
}
