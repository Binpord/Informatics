#include "std.h"
#include "CLine.h"

class CRect
{
private:
	CPoint hl, lr;

	CPoint getLeft() const;
	CPoint getRight() const;
	CPoint getCenter() const;
	bool IsIn(const CRect&) const;
public:
	CRect();
	~CRect();

	void setPoints(const CPoint&, const CPoint&);
	void print() const;
	void moveToPoint(CPoint);
	void mirrorV();
	void mirrorH();
	void rotate90();
	CRect* crossRect(const CRect&) const;
};
