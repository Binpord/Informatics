#include "std.h"
#include "CPoint.h"

class CLine 
{
protected:
	CPoint start;			// start is low left
	int length;
	bool ishor;

	int dot_interception(const CLine&, const CLine&, CLine*);
	int line_interception(const CLine&, const CLine&, int, CLine*);
	int make_line_interception(const CLine&, const CLine&, int, CLine*);
public:
	CLine();
	CLine(const CPoint&, const CPoint&);
	void GetLine();
	void SetLine(const CPoint&, const CPoint&);
	CPoint GetStart();
	int GetLength();
	bool IsHor();
	bool IsPoint();
	void print();
	int operator=(const CLine&);
	int Intercept(const CLine&, CLine* = NULL);
};
