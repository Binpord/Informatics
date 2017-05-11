#include "CFig.h"

/*
 * ==================================================
 * CFig part
 * ==================================================
 * CFig(Image*);
 * virtual void Draw() = 0;
 * virtual void MoveTo(const wxPoint&) = 0;
 * virtual void Fill(const wxColour&) = 0;
 * ==================================================
 */

CFig::CFig(Image* m)
{
	im = m;
}

/*
 * ==================================================
 * CLine part
 * ==================================================
 * CLine(Image*);
 * void SetPoints(const wxPoint&, const wxPoint&);
 * void SetColour(const wxColour&);
 * void Draw();
 * void MoveTo(const wxPoint&);
 * void Fill(const wxColour&);
 * ==================================================
 */

CLine::CLine(Image* m):CFig(m)
{}

void CLine::SetPoints(const wxPoint& a, const wxPoint& b)
{
	first = a, second = b;
}

void CLine::SetColour(const wxColour& cl)
{
	im->setPen(cl);
}

void CLine::Draw()
{
	im->DrawLine(first, second);
}
