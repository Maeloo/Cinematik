#pragma once

#include "../Utils/Vec3.h"
#include "Geometry.h"
#include <algorithm>
#include "../Transform/Transform.h"
#include <vector>

class Triangle : public Geometry {
protected:
	Vec3<float> Points[3];

public:
	Triangle ( void ) :Geometry ( ) { };
	Triangle ( Vec3<float> p1, Vec3<float> p2, Vec3<float> p3 );
	~Triangle ( void );

	float area ( ) const;
	float perimeter ( ) const;
	void shrink ( float t );

	void shrinkByDist ( float distance );
	void shrinkByDistForQuad ( float distance_ );

	Vec3<float> getMaxPoint ( );
	Vec3<float> getMinPoint ( );

	bool isIn(Vec3<float>);
	Vec3<float>* getPoints ( ) { return Points; }
};
