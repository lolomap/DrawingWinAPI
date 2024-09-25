#include "vec.h"
#include <math.h>

vec::vec()
{
    this->x = 0.;
    this->y = 0.;
}

vec::vec(double x, double y)
{
    this->x = x;
    this->y = y;
}

vec& vec::operator*=(const double value)
{
    this->x *= value;
    this->y *= value;
    return *this;
}

vec vec::operator*(double const& value)
{
    vec res;
    res.x = x * value;
    res.y = y * value;

    return res;
}

vec& vec::operator+=(const vec value)
{
    this->x += value.x;
    this->y += value.y;
    return *this;
}

vec vec::operator+(vec const& value)
{
    vec res;
    res.x = x + value.x;
    res.y = y + value.y;
    return res;
}

vec& vec::operator-=(const vec value)
{
    this->x -= value.x;
    this->y -= value.y;
    return *this;
}

vec vec::operator-(vec const& value)
{
    vec res;
    res.x = x - value.x;
    res.y = y - value.y;
    return res;
}

double vec::Dot(vec v)
{
    return x * v.x + y * v.y;
}

double vec::GetLength()
{
    return sqrt(x*x + y*y);
}

vec vec::Normalized()
{
    if (x == 0. && y == 0.) return vec(0., 0.);
    double length = GetLength();
    return vec(x / length, y / length);
}

double vec::GetDistance(vec p)
{
    return sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y));
}

void vec::Move(vec direction, double speed, double deltaTime)
{
    direction = direction.Normalized();
    x += direction.x * speed * deltaTime;
    y += direction.y * speed * deltaTime;
}