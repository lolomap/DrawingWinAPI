#pragma once
class vec
{
public:
	double x, y;

	vec();
	vec(double x, double y);

	vec& operator*=(const double value);
	vec operator*(double const& value);
	vec& operator+=(const vec value);
	vec operator+(vec const& value);
	vec& operator-=(const vec value);
	vec operator-(vec const& value);

	double Dot(vec v);
	double GetLength();
	vec Normalized();
	double GetDistance(vec p);

	void Move(vec direction, double speed, double deltaTime);
};

