#include "std.h"

class CFrac
{
private:
	int un, num, denom;

	void simplify(int*, int*);
public:
	int getUn();
	int getNum();
	int getDenom();
	CFrac();
	CFrac(int, int, int);
	void setVal(int, int, int);
	CFrac operator+(const CFrac&);
	CFrac& operator++(int);
	CFrac& operator!();
	int operator<(const CFrac&);
	int operator>(const CFrac&);
	int operator==(const CFrac&);

	void print();
};
