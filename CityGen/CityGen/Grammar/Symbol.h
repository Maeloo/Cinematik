#pragma once

#include "..\Mesh\Mesh.h"

class Symbol {

public:
	Symbol ( );
	~Symbol ( );

	virtual void Generate ( std::vector<Mesh> &mesh, int compteur ) const = 0;
};

