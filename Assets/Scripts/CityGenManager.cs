using UnityEngine;
using System.Collections;

public class CityGenManager : MonoBehaviour {

    public Material mat;

    private static int partCount;


    void Start ( ) {
        GenerateCityFromData ( );
    }


    void GenerateCityFromData ( ) {
        int meshCount;
        float[] verticesf;
        int[] vertice_sizes;
        int[] faces;
        int[] face_sizes;

        partCount = 0;

        CityGen.CityCSharpLibrary.GenerateCity ( out meshCount, out verticesf, out  vertice_sizes, out faces, out face_sizes );

        Vector3[] vertices = getVertices ( verticesf );


        int idx0 = 0;
        int idx1 = 0;
        for ( int i = 0; i < meshCount; i++ ) {
            Vector3[] vertices_mesh = new Vector3[vertice_sizes[i]];
            
            for ( int j = 0; j < vertice_sizes[i]; j++ ) {
                vertices_mesh[j] = vertices[idx0];
                idx0++;
            }

            int[] faces_mesh = new int[face_sizes[i]];

            for ( int k = 0; k < face_sizes[i]; k++ ) {
                faces_mesh[k] = faces[idx1];
                idx1++;
            }

            GenerateMesh ( vertices_mesh, faces_mesh, transform );
        }
    }


    void GenerateMesh ( Vector3[] vertices, int[] faces, Transform parent ) {
        GameObject part = new GameObject ( );
        part.name = "part_" + partCount.ToString ( "00" );
        part.transform.SetParent ( parent );

        ++partCount;

        Mesh mesh = new Mesh ( );
        mesh.Clear ( );
        mesh.vertices = vertices;
        mesh.triangles = faces;

        mesh.RecalculateNormals ( );

        part.AddComponent<MeshFilter> ( ).mesh = mesh;

        MeshRenderer meshRenderer = part.AddComponent<MeshRenderer> ( );
        meshRenderer.material = mat;
    }


    Vector3[] getVertices ( float[] points ) {
        int pointsSize = points.Length;
        Vector3[] vertices = new Vector3[pointsSize / 3];
        for ( int i = 0; i < pointsSize - 2; i += 3 ) {
            vertices[i / 3] = new Vector3 ( points[i], points[i + 1], points[i + 2] );
        }
        return vertices;
    }
	
}
