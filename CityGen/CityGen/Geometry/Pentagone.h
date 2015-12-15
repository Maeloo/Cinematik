#pragma once

#include "../Utils/Vec3.h"
#include "Geometry.h"
#include <algorithm>
#include "../Transform/Transform.h"

class Pentagone : public Geometry {
protected:
	Vec3<float> Points[5];

public:
	Pentagone ( void ) :Geometry ( ) { };
	Pentagone ( Vec3<float>, Vec3<float>, Vec3<float>, Vec3<float>, Vec3<float> );
	~Pentagone ( void );

	float TriangleAir ( Vec3<float>, Vec3<float>, Vec3<float> ) const;

	float area ( ) const;
	float perimeter ( ) const;
	void shrink ( float t );
};
