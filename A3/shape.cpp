#include "shape.h"


shape::shape(vec center, double halfWidth, double halfHeight)
{
	this->center.x = center.x;
	this->center.y = center.y;
	this->halfWidth = halfWidth;
	this->halfHeight = halfHeight;

	direction = vec(1, 0);
	speed = 0.;
}

double shape::GetRadius()
{
	return sqrt(halfWidth * halfWidth * 4. + halfHeight * halfHeight * 4.) / 2.;
}
