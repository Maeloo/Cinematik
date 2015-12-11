using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System.IO;


public class TerrainGenManager : MonoBehaviour {

    public int width;
    public int height;

    public Material mat;


    void Start ( ) {
        StartCoroutine ( GenerateTerrain ( ) );
    }


    public IEnumerator GenerateTerrain ( ) {
        float[] vertex;
        float[] normals;
        int[] faces;

        TerrainCSharpLibrary.TerrainCSharpLibrary.GenerateTerrainData ( width, height, out vertex, out normals, out faces );

        GameObject terrain = new GameObject ( );
        terrain.name = "terrain";

        Mesh mesh = new Mesh ( );
        mesh.Clear ( );
        mesh.vertices = getVertices ( vertex );
        //mesh.normals = getNormals ( normals );
        mesh.triangles = faces;

        foreach ( int idx in faces ) {
            Debug.Log ( idx );
        }

        terrain.AddComponent<MeshFilter> ( ).mesh = mesh;

        MeshRenderer meshRenderer = terrain.AddComponent<MeshRenderer> ( );
        meshRenderer.material = mat;

        yield return null;
    }


    private Vector2[] getUVs ( Vector3[] vertex ) {
        Vector2[] uvs = new Vector2[vertex.Length];
        for ( int i = 0; i < vertex.Length; i++ ) {
            float x = vertex[i].x == .0f ? .0f : 1.0f / vertex[i].x;
            float y = vertex[i].y == .0f ? .0f : 1.0f / vertex[i].y;
            uvs[i] = new Vector2 ( x, y );
        }
        return uvs;
    }


    private Vector3[] getNormals ( float[] points ) {
        int normalSize = points.Length;
        Vector3[] normals = new Vector3[normalSize];
        for ( int i = 0; i < normalSize - 2; i += 3 ) {
            normals[i] = new Vector3 ( points[i], points[i + 1], points[i + 2] );
        }
        return normals;
    }


    private Vector3[] getVertices ( float[] points ) {
        int verticeSize = points.Length;
        Vector3[] vertices = new Vector3 [ verticeSize ];
        for ( int i = 0; i < verticeSize - 2; i += 3 ) {
            vertices[i] = new Vector3 ( points[i], points[i + 1], points[i + 2]  / 100);
            Debug.Log ( vertices[i] );  
        }
        return vertices;
    }

}
