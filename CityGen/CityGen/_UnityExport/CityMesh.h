#pragma once

#include "..\Mesh\Mesh.h"
#include "..\Grammar\Map\QuadrangleSymbol.h"

#include <vector>

class CityMesh {
protected:
	std::vector<Mesh> list_meshs;

public:
	CityMesh ( );
	~CityMesh ( );

	std::vector<Mesh> create ( );
};

