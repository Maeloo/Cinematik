#include "CityCPPLibrary.h"

// CityCPPLibrary.cpp : Defines the exported functions for the DLL application.
//


#include <objbase.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "CityCPPLibrary.h"
#include "Mesh\Mesh.h"
#include "_UnityExport\CityMesh.h"


extern "C" {

	int TestDll ( ) {
		return 1;
	}


	void GenerateCityData (
		int *meshcount,
		float **vertices, int **vertexSizes,
		//float ***normals, int **normalSize,
		int **faces, int **faceSizes ) 
	{
		CityMesh cm = CityMesh ( );
		std::vector<Mesh> meshs = cm.create ( );

		*meshcount = meshs.size ( );

		*vertexSizes = new int[meshs.size ( )];
		*faceSizes = new int[meshs.size ( )];

		int total_v = 0;
		int total_f = 0;
#pragma omp parallel for
		for ( int i = 0; i < meshs.size ( ); ++i ) {
			int meshPointsSize = meshs[i].getPoints ( ).size ( ) * 3;
			( *vertexSizes )[i] = meshPointsSize;
			total_v += meshPointsSize;

			int meshFacesSize = meshs[i].getFaces ( ).size ( ) * 3;
			( *faceSizes )[i] = meshFacesSize;
			total_f += meshFacesSize;
		}

		*vertices = new float[total_v];
		*faces = new int[total_f];

#pragma omp parallel for
		for ( int i = 0; i < meshs.size ( ); ++i ) {
			for ( int j = 0; j < ( *vertexSizes )[i]; j += 3 ) {
				( *vertices )[j + i * ( *vertexSizes )[i]]		= meshs[i].getPoints ( )[j / 3].x;
				( *vertices )[j + i * ( *vertexSizes )[i] + 1]	= meshs[i].getPoints ( )[j / 3].y;
				( *vertices )[j + i * ( *vertexSizes )[i] + 2]	= meshs[i].getPoints ( )[j / 3].z;
			}

			for ( int j = 0; j < ( *faceSizes )[i] * 3; j += 3 ) {
				( *faces )[j + i * ( *faceSizes )[i]]		= meshs[i].getFaces ( )[j / 3].x;
				( *faces )[j + i * ( *faceSizes )[i] + 1]	= meshs[i].getFaces ( )[j / 3].y;
				( *faces )[j + i * ( *faceSizes )[i] + 2]	= meshs[i].getFaces ( )[j / 3].z;
			}
		}
	}

}
