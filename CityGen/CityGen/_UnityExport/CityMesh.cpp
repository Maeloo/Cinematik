#include "CityMesh.h"


CityMesh::CityMesh ( ) {
	list_meshs = std::vector<Mesh> ( );
}


CityMesh::~CityMesh ( ) {
}


std::vector<Mesh> CityMesh::create ( ) {
	QuadrangleSymbol qs = QuadrangleSymbol::genBorder ( Vec3<float> ( 0.f ), Vec3<float> ( 0.f, 500.f, 0.f ), Vec3<float> ( 500.f, 500.f, 0.f ), Vec3<float> ( 500.f, 0.f, 0.f ), 10.f, 3.f, 1.f, list_meshs, Vec3<float> ( 250.f, 250.f, 0.f ), Vec3<float> ( 500.f, 500.f, 0.f ) );
	qs.Generate ( list_meshs, 10 );
	return list_meshs;
}
