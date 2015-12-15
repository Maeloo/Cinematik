#include "Quadrangle.h"


Quadrangle::Quadrangle() : p1(Vec3<float>(0.f)), p2(Vec3<float>(0.f)), p3(Vec3<float>(0.f)), p4(Vec3<float>(0.f))
{
}

Quadrangle::Quadrangle(Vec3<float> p1_, Vec3<float> p2_, Vec3<float> p3_, Vec3<float> p4_) : p1(p1_), p2(p2_), p3(p3_), p4(p4_)
{
}

float Quadrangle::area() const
{
	
	float a = distance(p1, p2);
	float b = distance(p2, p3);
	float c = distance(p3, p4);
	float d = distance(p4, p1);
	float p = distance(p1, p3);
	float q = distance(p2, p4);
	//float s = (a + b + c + d) * 0.5;

	return(0.25f * sqrtf(4.f * p * p * q * q - (b * b + d * d - a * a - c * c) * (b * b + d * d - a * a - c * c)));


	//return 0.5 * sqrtf(Vec3<float>::dotProduct(p, p) * Vec3<float>::dotProduct(q, q) - (Vec3<float>::dotProduct(p, q) * Vec3<float>::dotProduct(p, q)));
}

float Quadrangle::perimeter() const
{
	return distance(p1, p2) + distance(p2, p3) + distance(p3, p4) + distance(p4, p1);
}

void Quadrangle::shrink(float t)
{
	Vec3<float> pivot = (p1+p2+p3+p4)*0.25f;

	Mat4x4 matrice = Transform::Shrink(t,pivot).m;

	p1 = matrice*p1;
	p2 = matrice*p2;
	p3 = matrice*p3;
	p4 = matrice*p4;
}

void Quadrangle::shrinkByDist ( float distance_ ) {
	Triangle t1 = Triangle ( p2, p1, p4 ); // T1;
	t1.shrinkByDistForQuad ( -distance_ );

	Triangle t2 = Triangle ( p2, p1, p3 ); // T2;
	t2.shrinkByDistForQuad ( -distance_ );

	Triangle t3 = Triangle ( p2, p4, p3 ); // T3;
	t3.shrinkByDistForQuad ( -distance_ );

	Triangle t4 = Triangle ( p1, p4, p3 ); // T4;
	t4.shrinkByDistForQuad ( -distance_ );

	p1 = t1.getPoints ( )[1];
	p2 = t2.getPoints ( )[0];
	p3 = t3.getPoints ( )[2];
	p4 = t4.getPoints ( )[1];
}

// Test si 2 quadrangle s'overlap (on les suppode de meme Z)
bool Quadrangle::intersect(Quadrangle q)
{
	return (Utils::isSegmentIntersect(p1.x, p1.y, p2.x, p2.y, q.p1.x, q.p1.y, q.p2.x, q.p2.y) ||
		Utils::isSegmentIntersect(p1.x, p1.y, p2.x, p2.y, q.p2.x, q.p2.y, q.p3.x, q.p3.y) ||
		Utils::isSegmentIntersect(p1.x, p1.y, p2.x, p2.y, q.p3.x, q.p3.y, q.p4.x, q.p4.y) ||
		Utils::isSegmentIntersect(p1.x, p1.y, p2.x, p2.y, q.p4.x, q.p4.y, q.p1.x, q.p1.y) ||

		Utils::isSegmentIntersect(p2.x, p2.y, p3.x, p3.y, q.p1.x, q.p1.y, q.p2.x, q.p2.y) ||
		Utils::isSegmentIntersect(p2.x, p2.y, p3.x, p3.y, q.p2.x, q.p2.y, q.p3.x, q.p3.y) ||
		Utils::isSegmentIntersect(p2.x, p2.y, p3.x, p3.y, q.p3.x, q.p3.y, q.p4.x, q.p4.y) ||
		Utils::isSegmentIntersect(p2.x, p2.y, p3.x, p3.y, q.p4.x, q.p4.y, q.p1.x, q.p1.y) ||

		Utils::isSegmentIntersect(p3.x, p3.y, p4.x, p4.y, q.p1.x, q.p1.y, q.p2.x, q.p2.y) ||
		Utils::isSegmentIntersect(p3.x, p3.y, p4.x, p4.y, q.p2.x, q.p2.y, q.p3.x, q.p3.y) ||
		Utils::isSegmentIntersect(p3.x, p3.y, p4.x, p4.y, q.p3.x, q.p3.y, q.p4.x, q.p4.y) ||
		Utils::isSegmentIntersect(p3.x, p3.y, p4.x, p4.y, q.p4.x, q.p4.y, q.p1.x, q.p1.y) ||

		Utils::isSegmentIntersect(p4.x, p4.y, p1.x, p1.y, q.p1.x, q.p1.y, q.p2.x, q.p2.y) ||
		Utils::isSegmentIntersect(p4.x, p4.y, p1.x, p1.y, q.p2.x, q.p2.y, q.p3.x, q.p3.y) ||
		Utils::isSegmentIntersect(p4.x, p4.y, p1.x, p1.y, q.p3.x, q.p3.y, q.p4.x, q.p4.y) ||
		Utils::isSegmentIntersect(p4.x, p4.y, p1.x, p1.y, q.p4.x, q.p4.y, q.p1.x, q.p1.y));
}

bool Quadrangle::isIn(Vec3<float> p)
{
	return Triangle(p1, p2, p3).isIn(p) || Triangle(p3, p1, p4).isIn(p);
}

bool Quadrangle::isIn(Quadrangle q)
{
	return isIn(q.p1) && isIn(q.p2) && isIn(q.p3) && isIn(q.p4);
}

bool Quadrangle::overlap(Quadrangle q)
{
	return isIn(q) || intersect(q) || q.isIn(*this) || q.intersect(*this);
}

Vec3<float> Quadrangle::getMinPoint()
{
	return p1.minv(p2).minv(p3).minv(p4);
}
Vec3<float> Quadrangle::getMaxPoint()
{
	return p1.maxv(p2).maxv(p3).maxv(p4);
}


Quadrangle Quadrangle::GenerateRectangle(Vec3<float> p0, Vec3<float> p1, float width, float height)
{
	if (Vec3<float>::dotProduct(p1, p1) > Vec3<float>::dotProduct(p0, p0))
	{
		Vec3<float> tmp(p0);
		p0 = p1;
		p1 = p0;
	}
	//Vec3<float> min = p0.min(p1);
	//Vec3<float> max = p0.max(p1);
	//return Quadrangle(p0, Vec3<float>(min.x, max.y, 0.f), p1, Vec3<float>(max.x, min.y, 0.f));//, p1 + dirP4 * width, p0 + dirP4 * width);
	Vec3<float> p0p1 = p1 - p0;
	p1 = p0 + p0p1.normalized() * height;
	p0p1 = p1 - p0;
	/*Vec3<float> tmp2 = tmp;*/
	Vec3<float> tmp3 = Vec3<float>(p0p1.y,  -p0p1.x, 0.f).normalized() * width;
	return Quadrangle(p0, p1, tmp3 + p0p1 + p0, tmp3 + p0);

}
bool Quadrangle::hasGoodNormal()
{
	return Vec3<float>::crossProduct(p4 - p1, p2 - p1).z > 0.f && Vec3<float>::crossProduct(p1 - p2, p3 - p2).z > 0.f  && Vec3<float>::crossProduct(p2 - p3, p4 - p3).z > 0.f && Vec3<float>::crossProduct(p3 - p4, p1 - p4).z > 0.f;
}
Quadrangle::~Quadrangle()
{
}
