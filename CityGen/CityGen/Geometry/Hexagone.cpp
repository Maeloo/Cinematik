#include "Hexagone.h"

Hexagone::Hexagone(Vec3<float> _Points[6]){

	for(int i=0;i<6;i++)
		Points[i] = _Points[i];

}

float Hexagone::TriangleAir(Vec3<float> p1,Vec3<float> p2,Vec3<float> p3) const{
	float a = distance(p1,p2);
	float b = distance(p2,p3);
	float c = distance(p1,p3);
	
	float s = 0.5f*(a+b+c);

	return sqrtf(s*(s-a)*(s-b)*(s-c));
}

float Hexagone::area() const{
	float a = distance(Points[0],Points[1]);
	float b = distance(Points[1],Points[2]);
	float c = distance(Points[2],Points[3]);
	float d = distance(Points[3],Points[4]);
	float e = distance(Points[4],Points[5]);
	float f = distance(Points[5],Points[0]);

	//Hexagone Regulier
	if(a==b && a==c && a==d && a==e && a==f){
		return ((3*sqrtf(3))/2)*(a*a); 
	}else{ //Hexagone Irregulier
		float AirTotal = 0;
		for(int i=1;i<5;i++)
			AirTotal +=TriangleAir(Points[0],Points[i],Points[i+1]);
		
		return AirTotal;
	}

}


float Hexagone::perimeter() const{
	float a = distance(Points[0],Points[1]);
	float b = distance(Points[1],Points[2]);
	float c = distance(Points[2],Points[3]);
	float d = distance(Points[3],Points[4]);
	float e = distance(Points[4],Points[5]);
	float f = distance(Points[5],Points[0]);

	return a+b+c+d+e+f;
}

//Je pense a revoir dans le cas de l'hexagone Concave
void Hexagone::shrink(float t){
	Vec3<float> pivot(0.f);
	for(int i=0;i<6;i++)
		pivot+=Points[i];
	pivot*=1.f/6.f;

	Mat4x4 matrice = Transform::Shrink(t,pivot).m;

	for(int i=0;i<6;i++)
	Points[i] = matrice*Points[i];
}

Hexagone::~Hexagone(void)
{
}
