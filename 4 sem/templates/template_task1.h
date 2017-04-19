#include "CPoint.h"

class CTwoPoints
{
protected:
	CPoint points[2];
public:
	CTwoPoints();
	CTwoPoints(int, int, int, int);
	ostream& put(ostream&);
	double getLength();
	bool operator>();	// needed for sort-tasks
	bool operator<();
}

class CIon
{
protected:
	char name[5];
	int mass;
public:
	CIon();
	CIon(char*, int);
	ostream& put(ostream&);
	bool operator>();	// needed for sort-tasks
	bool operator<();
}
