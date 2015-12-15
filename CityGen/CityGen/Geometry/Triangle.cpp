#include "Triangle.h"


Triangle::Triangle ( Vec3<float> p1_, Vec3<float> p2_, Vec3<float> p3_ )
{
	Points[0] = p1_;
	Points[1] = p2_;
	Points[2] = p3_;
}


float Triangle::area() const{
	
	float a = distance(Points[1],Points[2]);
	float b = distance(Points[0],Points[1]);
	float c = distance(Points[0],Points[2]);
	
	float s = 0.5f*(a+b+c);

	return sqrtf(s*(s-a)*(s-b)*(s-c));
}


float Triangle::perimeter() const{
	float a = distance(Points[1],Points[2]);
	float b = distance(Points[0],Points[1]);
	float c = distance(Points[0],Points[2]);

	return a+b+c;
}

	
void Triangle::shrink(float t){

	Vec3<float> pivot(0.f);
	for(int i=0;i<3;i++)
		pivot+=Points[i];
	pivot*=1.f/3.f;


	Mat4x4 matrice = Transform::Shrink(t,pivot).m;

	for(int i=0;i<3;i++)
	Points[i] = matrice*Points[i];
	
}


void Triangle::shrinkByDist ( float distance_ ) {
	Vec3<float> Q; // Centre du cercle inscris
	float a,b,c,
		R, // Rayon du cercle inscris
		k; // Coef d'homotéthie
	Vec3<float>
		A = Points[0],
		B = Points[1],
		C = Points[2];

	a = distance ( B, C );
	b = distance ( C, A );
	c = distance ( A, B );

	Q =
		( a / ( a + b + c ) ) * A +
		( b / ( a + b + c ) ) * B +
		( c / ( a + b + c ) ) * C;

	R = 2 * area ( ) / perimeter ( );

	k = R / ( R - distance_ );

	/*Points[0] = A + ( k - 1 ) * ( A - Q );
	Points[1] = B + ( k - 1 ) * ( B - Q );
	Points[2] = C + ( k - 1 ) * ( C - Q );*/
	Points[0] = A + ( k - 1 ) * ( Q - A );
	Points[1] = B + ( k - 1 ) * ( Q - B );
	Points[2] = C + ( k - 1 ) * ( Q - C );
}


void Triangle::shrinkByDistForQuad ( float distance_ ) {
	Vec3<float> Q; // Centre du cercle inscris
	float a, b, c,
		R, // Rayon du cercle inscris
		k; // Coef d'homotéthie
	Vec3<float>
		A = Points[0],
		B = Points[1],
		C = Points[2];

	a = distance ( B, C );
	b = distance ( C, A );
	c = distance ( A, B );

	Q =
		( a / ( a + b + c ) ) * A +
		( b / ( a + b + c ) ) * B +
		( c / ( a + b + c ) ) * C;

	R = 2 * area ( ) / perimeter ( );

	k = R / ( R - distance_ );

	Points[0] = A + ( k - 1 ) * ( A - Q );
	Points[1] = B + ( k - 1 ) * ( B - Q );
	Points[2] = C + ( k - 1 ) * ( C - Q );
}


bool Triangle::isIn(Vec3<float> p)
{
	Vec3<float> v0(Points[1] - Points[0]);
	Vec3<float> v1(Points[2] - Points[0]);
	Vec3<float> v2(p - Points[0]);

	// Compute dot products
	float dot00 = Vec3<float>::dotProduct(v0, v0);
	float dot01 = Vec3<float>::dotProduct(v0, v1);
	float dot02 = Vec3<float>::dotProduct(v0, v2);
	float dot11 = Vec3<float>::dotProduct(v1, v1);
	float dot12 = Vec3<float>::dotProduct(v1, v2);

	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return (u >= 0) && (v >= 0) && (u + v < 1);
}


Vec3<float> Triangle::getMinPoint ( ) {
	return Points[0].minv ( Points[1] ).minv ( Points[2] );
}


Vec3<float> Triangle::getMaxPoint ( ) {
	return Points[0].maxv ( Points[1] ).maxv ( Points[2] );
}


Triangle::~Triangle(void)
{
}
