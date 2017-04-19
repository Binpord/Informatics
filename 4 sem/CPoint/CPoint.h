#include "std.h"

class CPoint
{
private:
	int x, y;

public:
	CPoint();
	CPoint(int, int);
	void getVal();
	int GetX() const;
	int GetY() const;
	void moveOn(int, int);
	void setPoint(int, int);
	int diffX(const CPoint&) const;
	int diffY(const CPoint&) const;
	void print() const;
	void endllessprint() const;
	int operator=(CPoint);
	std::ostream& put(std::ostream&);
	std::ostream& operator<<(std::ostream&, CPoint&);
};
