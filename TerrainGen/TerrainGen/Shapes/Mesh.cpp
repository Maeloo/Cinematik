#include "Mesh.h"


Mesh::Mesh() : faces(std::vector<Triangle>())
{
}

bool Mesh::intersect(const Ray &ray, float * tHit) const
{
	float newt = INFINITY;
	float tmpt = INFINITY;;
	for (int i = 0; i < faces.size(); ++i)
	{
		if (faces[i].intersect(ray, &tmpt))
			newt = std::fminf(newt, tmpt);
	}
	*tHit = newt;
	return newt < INFINITY;
}

Mesh::Mesh ( const Terrain & terrain, float precision ) {
	float step	= 1.f / precision;
	vertex		= std::vector<Point> ( );
	normals		= std::vector<Normals> ( );
	facesIndex	= std::vector<int> ( );

	int height	= terrain.getHeight ( ) * precision;
	int width	= terrain.getWidth ( ) * precision;
	
	for ( int j = 0; j < height; j += step ) {
		for ( int i = 0; i < width; i += step ) {
			vertex.push_back ( terrain.getPoint ( i, j ) );
			
			normals.push_back ( terrain.getNormal ( terrain.getPoint ( i, j ) ) );
		}
	}

	for ( int j = 0; j < height - 1; j++ ) {
		for ( int i = 0; i < width - 1; i++ ) {
			int i0 = j * width + i;
			int i1 = i0 + width;

			facesIndex.push_back ( i0 );					
			facesIndex.push_back ( i0 + 1  );		
			facesIndex.push_back ( i1 + 1 );			

			facesIndex.push_back ( i1 + 1 );
			facesIndex.push_back ( i1 ); 
			facesIndex.push_back ( i0 );
		}
	}
}

BBox Mesh::getBound() const
{
	BBox res;
	for (int i = 0; i < faces.size(); ++i)
	{
		res = unionBBox(res, faces[i].getBound());
	}
	return res;
}

Normals Mesh::getNormal(Point p) const
{
	for (int i = 0; i < faces.size(); ++i)
	{
		if (faces[i].isIn(p))
			return faces[i].getNormal(p);
	}
	return Normals();

}

Mesh::~Mesh()
{
}
