// TerrainCPPLibrary.h

#ifdef TERRAINFUNCDLL_EXPORT
#define TERRAINFUNCDLL_API __declspec(dllexport) 
#else
#define TERRAINFUNCDLL_API __declspec(dllimport) 
#endif

#include <string>


extern "C" {
	TERRAINFUNCDLL_API int TestDll ( );

	TERRAINFUNCDLL_API void GenerateTerrainData (
		unsigned int width, unsigned int height,
		float **vertex, int *vertexSize,
		float **normals, int *normalSize,
		int **faces, int *faceSize,
		float precision );

	TERRAINFUNCDLL_API void GenerateTerrainHeights (
		unsigned int width, unsigned int height,
		float **heights, float *max );

	TERRAINFUNCDLL_API void GenerateTerrainMesh (
		unsigned int width, unsigned int height,
		const char *name, int *meshCount,
		float precision );

	TERRAINFUNCDLL_API  void GenerateTerrainMeshData (
		unsigned int width, unsigned int height,
		char*** data, int* size, 
		float precision );
}
