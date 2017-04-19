#include "std.h"
#include "CPoint.h"

class CLine 
{
protected:
	CPoint start;			// start is low left
	int length;
	bool ishor;

	int dot_interception(const CLine&, const CLine&, CLine*) const;
	int line_interception(const CLine&, const CLine&, int, CLine*) const;
	int make_line_interception(const CLine&, const CLine&, int, CLine*) const;
public:
	CLine();
	CLine(const CPoint&, const CPoint&);
	void GetLine();
	void SetLine(const CPoint&, const CPoint&);
	CPoint GetStart() const;
	int GetLength() const;
	bool IsHor() const;
	bool IsPoint() const;
	void print() const;
	int operator=(const CLine&);
	int Intercept(const CLine&, CLine* = NULL) const;
};
