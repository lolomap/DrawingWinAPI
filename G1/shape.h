#pragma once
#include <math.h>
#include "vec.h"

class shape
{
public:
	vec center;
	double halfWidth, halfHeight;

	vec direction;
	double speed;

	shape(vec center, double halfWidth, double halfHeight);

	double GetRadius();
};

