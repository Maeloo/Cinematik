#pragma once
#include "../../Geometry/Quadrangle.h"
#include "../../Mesh/Mesh.h"

class Toit {
protected:
	Quadrangle q;
	float h,rotate;
	int etages;

public:
	Toit(const Vec3<float>&, const Vec3<float>&, const Vec3<float>&, const Vec3<float>&, const float&, const float&,int);
	void G ( Mesh& ) const;
};
