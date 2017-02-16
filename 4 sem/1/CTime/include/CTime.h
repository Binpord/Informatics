#include "std.h"

class CTime
{
private:
	int h, min;
	CTime div(CTime, CTime);
public:
	CTime();
	void print();
	void setTime(int _h, int _min);
	CTime diff(CTime);
	int isLater(CTime);
	CTime add(CTime);
};
