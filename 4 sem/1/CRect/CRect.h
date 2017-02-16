#include "std.h"
#include "CPoint.h"

class CRect
{
private:
	CPoint hl, lr;

	CPoint getLeft();
	CPoint getRight();
public:
	CRect();
	~CRect();

	void setPoints(CPoint, CPoint);
	void print();
	void moveToPoint(CPoint a);
	void mirrorV();
	void mirrorH();
	void rotate90();
	CRect* crossRect(CRect a);
}
