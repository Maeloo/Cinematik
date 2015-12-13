#include "TerrainCPPLibrary.h"

// TerrainCPPLibrary.cpp : Defines the exported functions for the DLL application.
//


#include <objbase.h>
#include <stdlib.h>
#include <stdio.h>

#include "TerrainCPPLibrary.h"
#include "Terrain\TerrainFractal.h"
#include "Shapes\Mesh.h"
#include "Export\TerrainMesh.h"
#include "Export\MeshWriter.h"


extern "C" {

	int TestDll ( ) {
		return 1;
	}

	void GenerateTerrainMeshData (
		unsigned int width, unsigned int height,
		char*** data, int* size,
		float precision )
	{
		float step = 1.f / precision;
		TerrainFractal terrain = TerrainFractal ( width, height );

		std::vector<Vec3<float>> verticesData = std::vector<Vec3<float>> ( );

		for ( float y = 0; y < height; y+=step ) {
			for ( float x = 0; x < width; x += step ) {
				Point p = terrain.getPoint ( x, y );
				verticesData.push_back ( Vec3<float> ( p.x, p.y, p.z ) );
			}
		}

		std::vector<unsigned int> facesIndex = std::vector<unsigned int> ( );

		for ( float y = 0; y < ( height * precision ) - 1; y++ ) {
			for ( float x = 0; x < ( width * precision ) - 1; x++ ) {
				int i0 = y * ( width * precision ) + x;
				int i1 = i0 + ( width * precision );

				facesIndex.push_back ( i0 );
				facesIndex.push_back ( i0 + 1 );
				facesIndex.push_back ( i1 + 1 );

				facesIndex.push_back ( i1 + 1 );
				facesIndex.push_back ( i1 );
				facesIndex.push_back ( i0 );
			}
		}

		std::vector<TerrainMesh> meshs = TerrainMesh::create ( verticesData, facesIndex, false );

		*size = meshs.size ( );
		size_t sizeOfArray = sizeof ( char* ) * meshs.size ( );

		*data = ( char** )::CoTaskMemAlloc ( sizeOfArray );
		memset ( *data, 0, sizeOfArray );

		for ( int i = 0; i < meshs.size ( ); ++i ) {
			std::string res;
			MeshWriter::exportData ( meshs[i], res );
			
			const char* chr = res.c_str ( );
			
			( *data )[i] = ( char* )::CoTaskMemAlloc ( strlen ( chr ) + 1 );
			strcpy ( ( *data )[i], chr );
			
			chr = NULL;
			res = "";
		}

		return;
	}


	void GenerateTerrainMesh (
		unsigned int width, unsigned int height,
		const char *name, int *meshCount,
		float precision )
	{
		float step = 1.f / precision;
		TerrainFractal terrain = TerrainFractal ( width, height );

		std::vector<Vec3<float>> verticesData = std::vector<Vec3<float>> ( );

		for ( float y = 0; y < height; y += step ) {
			for ( float x = 0; x < width; x += step ) {
				Point p = terrain.getPoint ( x, y );
				verticesData.push_back ( Vec3<float> ( p.x, p.y, p.z ) );
			}
		}

		std::vector<unsigned int> facesIndex = std::vector<unsigned int> ( );

		for ( float y = 0; y < ( height * precision ) - 1; y++ ) {
			for ( float x = 0; x < ( width * precision ) - 1; x++ ) {
				int i0 = y * ( width * precision ) + x;
				int i1 = i0 + ( width * precision );

				facesIndex.push_back ( i0 );
				facesIndex.push_back ( i0 + 1 );
				facesIndex.push_back ( i1 + 1 );

				facesIndex.push_back ( i1 + 1 );
				facesIndex.push_back ( i1 );
				facesIndex.push_back ( i0 );
			}
		}

		std::vector<TerrainMesh> meshs = TerrainMesh::create ( verticesData, facesIndex, true );

		*meshCount = meshs.size ( );

		for ( int i = 0; i < meshs.size ( ); ++i ) {
			char numstr[21];
			sprintf_s ( numstr, "%d", i );
			std::string name_obj = name + std::string ( numstr ) + std::string ( ".obj" );
			MeshWriter::exportObj ( meshs[i], name_obj );
		}
	}


	void GenerateTerrainHeights (
		unsigned int width, unsigned int height,
		float **heights, float *max ) 
	{
		TerrainFractal terrain = TerrainFractal ( width, height );

		*heights = new float[height * width];

		*max = terrain.getHigh ( );

		float max_inv = 1.f / terrain.getHigh ( );
		for ( int x = 0; x < width; ++x ) {
			for ( int y = 0; y < height; ++y ) {
				
				( *heights )[x * height + y] = terrain.getPoint ( x, y ).z * max_inv;
			}
		}
	}


	void GenerateTerrainData ( 
		unsigned int width, unsigned int height, 
		float **vertex, int *vertexSize,
		float **normals, int *normalSize,
		int **faces, int *faceSize,
		float precision ) 
	{
		TerrainFractal terrain = TerrainFractal ( width, height );

		float step = 1.f / precision;
		std::vector<Point> terrainVertex = std::vector<Point> ( );
		std::vector<Normals> terrainNormals = std::vector<Normals> ( );
		std::vector<int> facesIndex = std::vector<int> ( );

		int h = height * precision;
		int w = width * precision;

		for ( float j = 0; j < height; j += step ) {
			for ( float i = 0; i < width; i += step ) {
				terrainVertex.push_back ( terrain.getPoint ( i, j ) );
				terrainNormals.push_back ( terrain.getNormal ( terrain.getPoint ( i, j ) ) );
			}
		}

		for ( int j = 0; j < h - 1; j++ ) {
			for ( int i = 0; i < w - 1; i++ ) {
				int i0 = j * w + i;
				int i1 = i0 + w;

				facesIndex.push_back ( i0 );
				facesIndex.push_back ( i0 + 1 );
				facesIndex.push_back ( i1 + 1 );

				facesIndex.push_back ( i1 + 1 );
				facesIndex.push_back ( i1 );
				facesIndex.push_back ( i0 );
			}
		}

		*vertexSize = terrainVertex.size ( ) * 3;
		*normalSize = terrainNormals.size ( ) * 3;
		*faceSize	= facesIndex.size ( );
	
		*vertex = new float[terrainVertex.size ( ) * 3];
		for ( int i = 0; i < terrainVertex.size ( ) * 3; i += 3 ) {
			( *vertex )[i] = terrainVertex[i / 3].x;
			( *vertex )[i + 1] = terrainVertex[i / 3].y;
			( *vertex )[i + 2] = terrainVertex[i / 3].z;
		}
		
		*normals = new float[terrainNormals.size ( ) * 3];
		for ( int i = 0; i < terrainNormals.size ( ) * 3; i += 3 ) {
			( *normals )[i] = terrainNormals[i / 3].x;
			( *normals )[i + 1] = terrainNormals[i / 3].y;
			( *normals )[i + 2] = terrainNormals[i / 3].z;
		}

		*faces = new int[facesIndex.size ( )];
		for ( int i = 0; i < facesIndex.size ( ); i++ ) {
			( *faces )[i] = facesIndex[i];
		}
	}

}
