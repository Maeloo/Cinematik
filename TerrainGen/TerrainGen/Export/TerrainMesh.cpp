#include "TerrainMesh.h"


TerrainMesh::TerrainMesh ( ) : points ( std::vector<Vec3<float>> ( ) ), faces ( std::vector<Vec3<unsigned int>> ( ) ) {
}


TerrainMesh::~TerrainMesh ( ) {
}


std::vector<Vec3<float>> TerrainMesh::getPoints ( ) const {
	return points;
}

std::vector<Vec3<unsigned int>> TerrainMesh::getFaces ( ) const {
	return faces;
}

std::vector<Vec3<float>> TerrainMesh::getTextures ( ) const {
	return textures;
}

std::vector<Vec3<float>> TerrainMesh::getFacesNormales ( ) const {
	return facesNormales;
}

std::vector<Vec3<unsigned int>> TerrainMesh::getFacesTextures ( ) const {
	return facesTextures;
}

std::vector<Vec3<unsigned int>> TerrainMesh::getFacesNormalesIndex ( ) const {
	return facesNormalesIndex;
}

std::vector<Vec3<float>> TerrainMesh::getVerticesNormales ( ) const {
	return verticesNormales;
}

/* Merge du mesh courant avec un autre
* m : mesh a fusionner
*/
void TerrainMesh::merge ( const TerrainMesh & m ) {
	std::vector<Vec3<unsigned int>> newFaces = m.getFaces ( );
	std::vector<Vec3<unsigned int>> newFacesTextures = m.getFacesTextures ( );
	std::vector<Vec3<unsigned int>> newFacesNormales = m.getFacesNormalesIndex ( );

	std::vector<Vec3<float>> newPoints = m.getPoints ( );
	std::vector<Vec3<float>> newTextures = m.getTextures ( );
	std::vector<Vec3<float>> newNormales = m.getFacesNormales ( );

	Vec3<unsigned int> offset ( points.size ( ) );

	for ( unsigned int i = 0; i < newFaces.size ( ); ++i )
		newFaces[i] += offset;
	for ( unsigned int i = 0; i < newFacesTextures.size ( ); ++i )
		newFacesTextures[i] += offset;
	for ( unsigned int i = 0; i < newFacesNormales.size ( ); ++i )
		newFacesNormales[i] += offset;

	faces.insert ( faces.end ( ), newFaces.begin ( ), newFaces.end ( ) );
	facesTextures.insert ( facesTextures.end ( ), newFacesTextures.begin ( ), newFacesTextures.end ( ) );
	facesNormalesIndex.insert ( facesNormalesIndex.end ( ), newFacesNormales.begin ( ), newFacesNormales.end ( ) );

	points.insert ( points.end ( ), newPoints.begin ( ), newPoints.end ( ) );
	textures.insert ( textures.end ( ), newTextures.begin ( ), newTextures.end ( ) );
	facesNormales.insert ( facesNormales.end ( ), newNormales.begin ( ), newNormales.end ( ) );
}

std::vector<Vec3<float>>  TerrainMesh::calculateReelNormals ( const std::vector<Vec3<unsigned int>> &faces, const std::vector<Vec3<float>> &points ) {
	// Calcule des normales par face
	unsigned int facesCount = faces.capacity ( );
	std::vector<Vec3<unsigned int>> facesNormalesIndex = std::vector<Vec3<unsigned int>> ( facesCount );
	std::vector<Vec3<float>> facesNormales = std::vector<Vec3<float>> ( facesCount );

	for ( unsigned int i = 0; i < facesCount; ++i ) {
		Vec3<unsigned int> face = faces[i];
		Vec3<float> normal = Vec3<float>::crossProduct ( points[face.y] - points[face.x], points[face.z] - points[face.x] ).normalized ( );
		facesNormalesIndex[i] = face;
		facesNormales[i] = normal;
	}

	// Calcule des normales par vertex
	unsigned int verticesCount = points.capacity ( );
	std::vector<Vec3<float>> verticesNormales = std::vector<Vec3<float>> ( verticesCount );

	for ( unsigned int i = 0; i < verticesCount; ++i ) {
		Vec3<float> normal = Vec3<float> ( .0f, .0f, .0f );

		for ( unsigned int j = 0; j < facesCount; ++j ) {
			Vec3<unsigned int> face = faces[j];

			for ( unsigned int k = 0; k < verticesCount; ++k ) {
				if ( ( k == 0 && face.x == i ) ||
					 ( k == 1 && face.y == i ) ||
					 ( k == 2 && face.z == i ) ) {
					normal += facesNormales[j];
				}
			}

			verticesNormales[i] = normal.normalized ( );
		}
	}

	return verticesNormales;
}

std::vector<TerrainMesh> TerrainMesh::create ( const std::vector<Vec3<float>> &verticesData, const std::vector<unsigned int> &facesIndex, bool calculateNormals ) {
	std::vector<TerrainMesh> meshs = std::vector<TerrainMesh> ( );

	TerrainMesh mesh = TerrainMesh ( );
	for ( int i = 0; i < facesIndex.size ( ) - 2; i += 3 ) {
		if ( mesh.points.size ( ) < 65497 ) {
			Vec3<float> p1 = verticesData[facesIndex[i]];
			Vec3<float> p2 = verticesData[facesIndex[i + 1]];
			Vec3<float> p3 = verticesData[facesIndex[i + 2]];
			mesh.merge ( TerrainMesh::Triangle ( p1, p2, p3 ) );
		}
		else {
			if ( calculateNormals )
				mesh.verticesNormales = TerrainMesh::calculateReelNormals ( mesh.faces, mesh.points );

			meshs.push_back ( mesh );

			mesh = TerrainMesh ( );
			i -= 3;
		}
	}

	return meshs;
}

/*
* return : Mesh triangle
* p0, p1, p2 : sommet du triangle
*/
TerrainMesh TerrainMesh::Triangle ( const Vec3<float>& p0, const Vec3<float>& p1, const Vec3<float>& p2 ) {
	std::vector<Vec3<float>> points ( 3 );
	points[0] = p0;
	points[1] = p1;
	points[2] = p2;

	std::vector<Vec3<unsigned int>> faces ( 1 );
	faces[0] = Vec3<unsigned int> ( 1, 2, 3 );

	return TerrainMesh ( points, faces );
}
