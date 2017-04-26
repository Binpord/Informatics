#include "myImage.h"

bool myImage::operator==(const myImage& comp)
{
	return (this->pen == comp.pen && this->fill == comp.fill && this->w == comp.w && this->h == comp.h);
}
