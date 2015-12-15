#pragma once
#include "../../Geometry/Quadrangle.h"
#include "../../Mesh/Mesh.h"
#include "Toit.h"

class EtageCircle
{
protected:
	Quadrangle q;
	float h;
	float pourcentage; //Pour eviter une creation de bulding trop haute
	int etages;
public:
	EtageCircle(const Vec3<float>&, const Vec3<float>&, const Vec3<float>&, const Vec3<float>&, const float&, const float&, const int&);
	void G(Mesh&) const;

};




