#include "std.h"
#include "image.h"
#include <wx/gdicmn.h>		// for wxRect and wxPoint

class CFig
{
private:
	wxPoint center;
protected:
	Image* im;
public:
	CFig(Image*);
	virtual void Draw() = 0;
	virtual void MoveTo(const wxPoint&) = 0;
	virtual void Fill(const wxColour&) = 0;
};

class CLine:public CFig
{
private:
	wxPoint first, second;
public:
	CLine(Image*);
	void SetPoints(const wxPoint&, const wxPoint&);
	void SetColour(const wxColour&);
	void Draw();
	void MoveTo(const wxPoint&);
	void Fill(const wxColour&);
};

class CRect:public CFig
{
private:
	wxPoint topLeft, bottomRight;
public:
	CRect(Image*);
	void SetPoints(const wxPoint&, const wxPoint&);
	void SetColour(const wxColour&);
	void Draw();
	void MoveTo(const wxPoint&);
	void Fill(const wxColour&);
}
