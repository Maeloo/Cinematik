#include "Circle.h"

Circle::Circle() 
{ 
	center = Vec3<float>(.0f);
	radius = .0f;
}
Circle::Circle(const Vec3<float> o, const float &r)
{ 
	center = o;
	radius = r; 
}

float Circle::area() const 
{
	return radius * radius * Constantes::PI; 
}
float Circle::perimeter() const 
{ 
	return 2.0f * radius * Constantes::PI;
}
void Circle::shrink(float t) 
{ 
	radius -= t; 
}