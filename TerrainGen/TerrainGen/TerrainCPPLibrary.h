// TerrainCPPLibrary.h

#ifdef TERRAINFUNCDLL_EXPORT
#define TERRAINFUNCDLL_API __declspec(dllexport) 
#else
#define TERRAINFUNCDLL_API __declspec(dllimport) 
#endif

extern "C" {
	TERRAINFUNCDLL_API float TestDll ( );

	TERRAINFUNCDLL_API void GenerateTerrainMesh (
		unsigned int width, unsigned int height,
		float **vertex, int *vertexSize,
		float **normals, int *normalSize,
		int **faces, int *faceSize  );
}
