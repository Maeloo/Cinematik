#include "TerrainCPPLibrary.h"

// TerrainCPPLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "TerrainCPPLibrary.h"
#include "Terrain\TerrainFractal.h"
#include "Shapes\Mesh.h"


extern "C" {

	float TestDll ( ) {
		return 1.f;
	}


	void GenerateTerrainMesh ( 
		unsigned int width, unsigned int height, 
		float **vertex, int *vertexSize,
		float **normals, int *normalSize,
		int **faces, int *faceSize ) 
	{
		TerrainFractal terrain = TerrainFractal ( width, height );
		Mesh mesh = Mesh ( terrain, 1.f );

		*vertexSize = mesh.vertex.size ( ) * 3;
		*normalSize = mesh.normals.size ( ) * 3;
		*faceSize	= mesh.facesIndex.size ( );
	
		*vertex = new float[mesh.vertex.size ( ) * 3];
		for ( int i = 0; i < mesh.vertex.size ( ) * 3; i += 3 ) {
			(*vertex)[i] = mesh.vertex[i / 3].x;
			(*vertex)[i + 1] = mesh.vertex[i / 3].y;
			(*vertex)[i + 2] = mesh.vertex[i / 3].z;
		}
		
		*normals = new float[mesh.normals.size ( ) * 3];
		for ( int i = 0; i < mesh.normals.size ( ) * 3; i += 3 ) {
			(*normals)[i] = mesh.normals[i / 3].x;
			(*normals)[i + 1] = mesh.normals[i / 3].y;
			(*normals)[i + 2] = mesh.normals[i / 3].z;
		}

		*faces = new int[mesh.facesIndex.size ( )];
		for ( int i = 0; i < mesh.facesIndex.size ( ); i++ ) {
			(*faces)[i] = mesh.facesIndex[i];
		}
	}

}
