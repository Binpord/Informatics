#include "ImageRect.h"

ImageRect::ImageRect():wxRect()
{
	this->fill = wxColour(0, 0, 0);		// black by default
}

ImageRect::ImageRect(const wxPoint& topLeft, const wxPoint& bottomRight, const wxColour& in_fill):wxRect(topLeft, bottomRight)
{
	this->fill = in_fill;
}

bool ImageRect::operator==(const ImageRect& comp) const
{
	return ((wxRect)*this == (wxRect)comp) && (this->fill == comp.fill);
}

const ImageRect& ImageRect::operator=(const ImageRect& comp)
{
	*((wxRect*)this) = (wxRect)comp;
	this->fill = comp.fill;
	return *this;
}

void ImageRect::SetFill(int Red, int Green, int Blue)
{
	this->fill.Set(Red, Green, Blue);
}

wxColour ImageRect::GetFill() const
{
	return this->fill;
}
