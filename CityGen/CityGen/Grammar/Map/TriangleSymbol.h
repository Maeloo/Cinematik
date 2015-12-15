#pragma once


#include "..\..\Utils\Vec3.h"
#include "..\Symbol.h"
#include "..\Map\QuadrangleSymbol.h"
#include "..\..\Geometry\Triangle.h"
#include "..\..\Geometry\Circle.h"
#include "..\..\Utils\Utils.h"
#include <list>

class TriangleSymbol :
	public Symbol {
protected:
	Vec3<float> p1, p2, p3, mid, loin;

public:
	TriangleSymbol ( const Vec3<float> &, const Vec3<float> &, const Vec3<float> &, const Vec3<float> &, const Vec3<float> & );
	
	void Generate ( std::vector<Mesh> &mesh, int compteur ) const;

	static void addTrees ( Triangle q, std::vector<Mesh> & m, const std::list<Quadrangle>& rdcs, int nbTryTree );
	static TriangleSymbol genBorder ( const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>&p3, const float& borderSize, const float& sizePavement, const float& hPavement, Mesh& m, const Vec3<float>& _mid, const Vec3<float> & _loin );
	static bool checkNormal ( Triangle t );
	static Circle incircle ( Triangle t );
	static Quadrangle randomQuadInCircle ( Circle c );
};

