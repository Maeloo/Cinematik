#pragma once
#include "../../Geometry/Quadrangle.h"
#include "../../Mesh/Mesh.h"
#include "Toit.h"

class Etage {
protected:
	Quadrangle q;
	float h,rotate;
	float pourcentage; //Pour eviter une creation de bulding trop haute
	int type; //0= normal -- 1= twisted
	bool duo; //to check if there is already the building cut in 2 buildings
	int etages;
	float window_size;
public:
	Etage(const Vec3<float>&, const Vec3<float>&, const Vec3<float>&, const Vec3<float>&, const float&, const float&, const int&, const float&, const bool&, const int&, const float&);
	void G(Mesh&) const;
	static void addWindows(Mesh& m, const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const Vec3<float>& p4, const float& h, const int& etage, const float& win_size);
	static void addWindowsSide(Mesh& m, const Vec3<float>& p1, const Vec3<float>& p2, const float& hauteur, const float& shrink, const int& etage, const float& win_size);
};
