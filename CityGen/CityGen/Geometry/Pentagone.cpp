#include "Pentagone.h"


Pentagone::Pentagone ( Vec3<float> a, Vec3<float> b, Vec3<float> c, Vec3<float> d, Vec3<float> e ) {
	Points[0] = a;
	Points[1] = b;
	Points[2] = c;
	Points[3] = d;
	Points[4] = e;
}

float Pentagone::TriangleAir(Vec3<float> p1,Vec3<float> p2,Vec3<float> p3) const{
	float a = distance(p1,p2);
	float b = distance(p2,p3);
	float c = distance(p1,p3);
	
	float s = 0.5f*(a+b+c);

	return sqrtf(s*(s-a)*(s-b)*(s-c));
}

float Pentagone::area() const{
	float a = distance(Points[0],Points[1]);
	float b = distance(Points[1],Points[2]);
	float c = distance(Points[2],Points[3]);
	float d = distance(Points[3],Points[4]);
	float e = distance(Points[5],Points[0]);

	//Hexagone Regulier
	if(a==b && a==c && a==d && a==e){
		return ((3*sqrtf(3))/2)*(a*a); 
	}else{ //Hexagone Irregulier
		float AirTotal = 0;
		for(int i=1;i<4;i++)
			AirTotal +=TriangleAir(Points[0],Points[i],Points[i+1]);
		
		return AirTotal;
	}
}

float Pentagone::perimeter() const {
	float a = distance(Points[0],Points[1]);
	float b = distance(Points[1],Points[2]);
	float c = distance(Points[2],Points[3]);
	float d = distance(Points[3],Points[4]);
	float e = distance(Points[4],Points[0]);

	return a+b+c+d+e;
}

void Pentagone::shrink(float t){
	Vec3<float> pivot(0.f);
	for(int i=0;i<5;i++)
		pivot+=Points[i];
	pivot*=1.f/5.f;

	Mat4x4 matrice = Transform::Shrink(t,pivot).m;

	for(int i=0;i<5;i++)
	Points[i] = matrice*Points[i];
}


Pentagone::~Pentagone(void)
{
}
