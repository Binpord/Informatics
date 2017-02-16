#include "std.h"

class CPoint
{
private:
	int x, y;

public:
	CPoint();
	void getVal();
	void moveOn(int x1, int y1);
	int diffX(CPoint a);
	int diffY(CPoint a);
	void print();
};
