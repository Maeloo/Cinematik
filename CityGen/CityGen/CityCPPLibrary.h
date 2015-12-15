// CityCPPLibrary.h

#ifdef CITYFUNCDLL_EXPORT
#define CITYFUNCDLL_API __declspec(dllexport) 
#else
#define CITYFUNCDLL_API __declspec(dllimport) 
#endif

#include <string>


extern "C" {
	CITYFUNCDLL_API int TestDll ( );

	CITYFUNCDLL_API void GenerateCityData (
		int *meshcount,
		float **vertices, int **vertexSizes,
		//float ***normals, int **normalSize, // On verra plus tard
		int **faces, int **faceSizes );
}
