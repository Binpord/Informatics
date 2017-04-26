#include "image.h"

class myImage:public Image
{
public:
	bool operator==(const myImage&);
};
