#pragma once

#include <vector>

#include "..\Terrain\Terrain.h"
#include "Vec3.h"

class TerrainMesh {
protected:
	/*
	* Vecteur des coordonnes des points du mesh
	*/
	std::vector<Vec3<float>> points;

	/*
	* Vecteur des faces du mesh, contenant les indices des points
	*/
	std::vector<Vec3<unsigned int>> faces;

	/*
	* Vecteur des faces du mesh, contenant les indices des coordonnees de textures
	*/
	std::vector<Vec3<unsigned int>> facesTextures;

	/*
	* Vecteur des faces du mesh, contenant les indices des coordonnees des normales par face
	*/
	std::vector<Vec3<unsigned int>> facesNormalesIndex;

	/*
	* Vecteur des coordonnees de textures du mesh
	*/
	std::vector<Vec3<float>> textures;

	/*
	* Vecteur des coordonnees de normales du mesh par face
	*/
	std::vector<Vec3<float>> facesNormales;

	/*
	* Vecteur des coordonnees de normales du mesh par vertices
	*/
	std::vector<Vec3<float>> verticesNormales;

public:
	TerrainMesh ( );
	TerrainMesh ( std::vector<Vec3<float>> &points, std::vector<Vec3<unsigned int>> &faces ) : points ( points ), faces ( faces ) { }
	~TerrainMesh ( );

	std::vector<Vec3<float>> getPoints ( ) const;
	std::vector<Vec3<unsigned int>> getFaces ( ) const;
	std::vector<Vec3<float>> getTextures ( ) const;
	std::vector<Vec3<float>> getFacesNormales ( ) const;
	std::vector<Vec3<unsigned int>> getFacesTextures ( ) const;
	std::vector<Vec3<unsigned int>> getFacesNormalesIndex ( ) const;
	std::vector<Vec3<float>> getVerticesNormales ( ) const;

	static std::vector<Vec3<float>> calculateReelNormals ( const std::vector<Vec3<unsigned int>> &faces, const std::vector<Vec3<float>> &points );

	static TerrainMesh Triangle ( const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3 );

	static std::vector<TerrainMesh> create ( const std::vector<Vec3<float>> &verticesData, const std::vector<unsigned int> &facesIndex, bool calculateNormals );

	/* Merge du mesh courant avec un autre
	* m : mesh a fusionner
	*/
	void merge ( const TerrainMesh & m );
};

