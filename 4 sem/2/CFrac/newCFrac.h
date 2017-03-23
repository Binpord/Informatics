#include "std.h"

class CFrac
{
private:
	int num, denom;

	int simplify(int*, int*);
	int normalize();
public:
	int getNum();
	int getDenom();
	CFrac();
	CFrac(int, int, int);
	CFrac(int, int);
	void setVal(int, int, int);
	void setVal(int, int);
	CFrac operator-(const CFrac&);
	CFrac operator+(const CFrac&);
	CFrac operator+(int);
	CFrac& operator!();
	CFrac operator*(const CFrac&);
	CFrac operator/(const CFrac&);
	int operator=(const CFrac&);
	int operator<(const CFrac&);
	int operator>(const CFrac&);
	int operator==(const CFrac&);

	void print();
};
