#include "template_task1.h"

CTwoPoints::CTwoPoints()
{}

CTwoPoints::CTwoPoints(int x_1, int y_1, int x_2, int y_2)
{
	points[0].setPoint(x_1, y_1);
	points[1].setPoint(x_2, y_2);
}

ostream& CTwoPoints::put(ostream& s)
{
	std::cout << points[0] << ", " << points[1] << std::endl;
	return s;
}

int main()
{
}
