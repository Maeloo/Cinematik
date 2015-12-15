#pragma once

#include "../Utils/Vec3.h"
#include "Geometry.h"
#include <algorithm>
#include "../Transform/Transform.h"

class Hexagone : public Geometry {
protected:
	Vec3<float> Points[6];

public:
	Hexagone ( void ) :Geometry ( ) { };
	Hexagone ( Vec3<float>[] );
	~Hexagone ( void );

	float TriangleAir ( Vec3<float>, Vec3<float>, Vec3<float> ) const;

	float area ( ) const;
	float perimeter ( ) const;
	void shrink ( float t );
};
