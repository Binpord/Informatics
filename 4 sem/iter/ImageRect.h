#include <wx/gdicmn.h>		// for wxRect and wxPoint
#include <wx/colour.h>		// for wxColour

class ImageRect:public wxRect
{
protected:
	wxColour fill;
public:
	ImageRect();
	ImageRect(const wxPoint&, const wxPoint&, const wxColour&);
	bool operator==(const ImageRect&) const;
	const ImageRect& operator=(const ImageRect&);
	void SetFill(int, int, int);
	wxColour GetFill() const;
};
