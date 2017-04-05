#include "std.h"

class CPoint
{
private:
	int x, y;

public:
	CPoint();
	CPoint(int, int);
	void getVal();
	int GetX();
	int GetY();
	void moveOn(int, int);
	void setPoint(int, int);
	int diffX(const CPoint&);
	int diffY(const CPoint&);
	void print();
	void endllessprint();
	int operator=(CPoint);
};
