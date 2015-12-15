#pragma once
#include "Etage.h"
#include "Toit.h"
#include "EtageCircle.h"
#include "../../Mesh/Mesh.h"


class RDC {

protected:
	Quadrangle q;
	float h,dif;
	int type;
public:
	RDC(const Vec3<float>&, const Vec3<float>&, const Vec3<float>&, const Vec3<float>&, const float&, const float&, const int&);
	void G ( std::vector<Mesh>& ) const;
	static void addDoor(Mesh& m, const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const Vec3<float>& p4, const float& h);
};
