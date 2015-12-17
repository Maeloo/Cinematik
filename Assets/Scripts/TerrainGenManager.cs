using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.Linq;


public enum Method {
    FromData,
    UnityTerrain,
    FromObj,
    FromObjData
}


public class TerrainGenManager : MonoBehaviour {

    public Method method;
    public string name;
    
    [Range ( 100, 16000 )]
    public int size;
    
    [Range ( 1, 100 )]
    public int precision;
    
    public Material mat;


    public bool keepObj;
    public Texture2D tex;

    private static int partCount;

    private int reel_precision;
    private int reel_size;


    void Start ( ) {
        GenerateTerrain ( );
    }


    void checkSize ( ) {
        reel_size = size - size % precision;   
    }


    void checkPrecision ( ) {
        reel_precision = precision;
    }


    private float _chrono;
    void GenerateTerrain ( ) {
        checkSize ( );
        checkPrecision ( );
        Debug.Log ( "Reel size " + reel_size );
        Debug.Log ( "Reel precision " + reel_precision );

        _chrono = Time.realtimeSinceStartup;
        switch ( method ) {
            case Method.FromData:
                GenerateTerrainFromData ( );
                break;

            case Method.UnityTerrain:
                GenerateUnityTerrain ( );
                break;

            case Method.FromObj:
                GenerateTerrainFromObj ( );
                break;

            case Method.FromObjData:
                GenerateTerrainFromObjData ( );
                break;
        }

        float timed = Time.realtimeSinceStartup - _chrono;
        Debug.logger.Log ( "Generation time : " + timed.ToString ( ) );

        transform.Rotate ( Vector3.right, -90 );
    }


    void OnDestroy ( ) {
        if ( !keepObj && System.IO.Directory.Exists ( Path.Combine ( Application.dataPath, "Temp/" + name + "/" ) ) ) {
            System.IO.Directory.Delete ( Path.Combine ( Application.dataPath, "Temp/" + name + "/" ), true );
        }

        foreach ( Transform child in transform ) {
            Mesh mesh = child.GetComponent<MeshFilter> ( ).mesh;
            mesh.Clear ( );
            Destroy ( mesh );
        }

        Caching.CleanCache ( );
        
        System.GC.Collect ( );
    }


    // Marche plus ?
    public void GenerateTerrainFromObjData ( ) {
        string[] data;

        TerrainGen.TerrainCSharpLibrary.GenerateTerrainMeshData ( reel_size, reel_size, out data, 1.0f );

        GameObject obj = new GameObject ( );
        obj.name = name;

        for ( int i = 0; i < data.Length; ++i ) {
            StartCoroutine ( loadData ( obj.transform, data[i], i ) );
        }
    }


    IEnumerator loadData ( Transform parent, string data, int cpt ) {
        GameObject part = new GameObject ( );
        part.name = "part_" + cpt.ToString ( "00" );
        part.transform.SetParent ( parent );

        objReaderCSharpV4 objReader = part.AddComponent<objReaderCSharpV4> ( );

        yield return StartCoroutine ( objReader.Init ( data ) );

        Mesh mesh = objReader.mesh;

        part.AddComponent<MeshFilter> ( ).mesh = mesh;

        MeshRenderer meshRenderer = part.AddComponent<MeshRenderer> ( );
        meshRenderer.material = mat;
    }


    // Marche plus
    string _newFolderPath = "Assets/Temp/";
    public void GenerateTerrainFromObj ( ) {
        int meshCount = 0;
        
        _newFolderPath += name;

        string path = Path.Combine ( Application.dataPath, "Temp/" + name + "/" );
        System.IO.Directory.CreateDirectory ( path );

        TerrainGen.TerrainCSharpLibrary.GenerateTerrainObjs ( reel_size, reel_size, _newFolderPath + "/" + name + "_", ref meshCount, 1.0f );

#if UNITY_EDITOR
        AssetDatabase.Refresh ( );
#endif

        GameObject obj = new GameObject ( );
        obj.name = name;        

        for ( int i = 0; i < meshCount; ++i ) {
            StartCoroutine ( loadMesh ( obj.transform, path, i ) );
        }
    }


    IEnumerator loadMesh ( Transform parent, string path, int cpt ) {
        using ( WWW www = new WWW ( "file:///" + path + name + "_" + cpt + ".obj" ) ) {
            GameObject part = new GameObject ( );
            part.name = "part_" + cpt.ToString ( "00" );
            part.transform.SetParent ( parent );

            yield return www;

            if ( www.isDone && string.IsNullOrEmpty ( www.error ) ) {
                objReaderCSharpV4 objReader = part.AddComponent<objReaderCSharpV4> ( );
                
                yield return StartCoroutine ( objReader.Init ( www.text ) );

                Mesh mesh = objReader.mesh;

                part.AddComponent<MeshFilter> ( ).mesh = mesh;

                MeshRenderer meshRenderer = part.AddComponent<MeshRenderer> ( );
                meshRenderer.material = mat;

                DestroyImmediate ( objReader );
            } else {
                Debug.LogError ( www.error );
            }    
        }
    }


    // Marche mais rendu pas ouf
    void GenerateUnityTerrain ( ) {
        float[,] heightmap;

        float height = 0;
        TerrainGen.TerrainCSharpLibrary.GenerateTerrainHeightmap ( reel_size, reel_size, out heightmap, ref height );

        TerrainData data = new TerrainData ( );
        
        data.size = new Vector3 ( size / height, height, size / height ); // A revoir
        data.heightmapResolution = size;
        SplatPrototype sp = new SplatPrototype ( );
        sp.tileSize = new Vector2 ( size, size );
        sp.texture = tex;
        data.splatPrototypes = new SplatPrototype[] { sp };
        
        data.SetHeights ( 0, 0, heightmap );
        
        GameObject terrain = Terrain.CreateTerrainGameObject ( data );
    }


    void GenerateTerrainFromData ( ) {
        float[] vertex;
        float[] normals;
        int[] faces;

        TerrainGen.TerrainCSharpLibrary.GenerateTerrainData ( reel_size, reel_size, out vertex, out normals, out faces, 1.0f );

        partCount = 0;

        Vector3[] vertices = getVertices ( vertex );

        System.Array.Clear ( faces, 0, normals.Length );
        faces = null;

        System.Array.Clear ( normals, 0, normals.Length );
        normals = null;

        System.Array.Clear ( vertex, 0, vertex.Length );
        vertex = null;

        System.GC.Collect ( );

        //SplitDataIn4Quad ( terrain, vertices, faces );
        SplitDataIn2 ( applyPrecision ( vertices, reel_size, reel_size ), reel_size / reel_precision, reel_size / reel_precision );
    }


    void SplitDataIn2 ( Vector3[] verticesArray, int width, int height ) {
        if ( verticesArray.Length > 65000 ) {
            List<Vector3> vertices = verticesArray.ToList<Vector3> ( );

            System.Array.Clear ( verticesArray, 0, verticesArray.Length );
            verticesArray = null;

            int new_height1   = ( height / 2 ) + 1;
            int new_size1     = ( new_height1 * width );

            List<Vector3> vertices1 = vertices.GetRange ( 0, new_size1 );
            //List<int> faces1 = regenerateFaces ( width, new_height1 );

            int new_height2   = ( height - new_height1 ) + 1;
            int new_size2     = ( new_height2 * width );

            List<Vector3> vertices2 = vertices.GetRange ( new_size1 - width, new_size2 );
            //List<int> faces2 = regenerateFaces ( width, new_height2 );

            vertices.Clear ( );
            vertices = null;

            System.GC.Collect ( );

            SplitDataIn2 ( vertices1.ToArray ( ), width, new_height1 );
            SplitDataIn2 ( vertices2.ToArray ( ), width, new_height2 );
        } else {
            GenerateMesh ( verticesArray, regenerateFaces ( width, height ), transform );
        }
    }


    // Marche pas : à reprendre plus tard
    void SplitDataIn4Quad ( Vector3[] vertices, int[] faces ) {
        if ( vertices.Length > 65000 ) { 
            int idx = 0;
            int midSize = ( size + 1 ) / 2;
            int i0 = midSize - 1;
            int i1 = size * ( size - 1 ) / 2;
            int i2 = i1 + midSize - 1;

            Vector3[] vertices0 = new Vector3[midSize * midSize];
            Vector3[] vertices1 = new Vector3[midSize * midSize];
            Vector3[] vertices2 = new Vector3[midSize * midSize];
            Vector3[] vertices3 = new Vector3[midSize * midSize];

            for ( int j = 0; j < midSize; ++j ) {
                for ( int i = 0; i < midSize; ++i ) {
                    vertices0[idx] = vertices[i + size * j];
                    vertices1[idx] = vertices[i + i0 + size * j];
                    vertices2[idx] = vertices[i + i1 + size * j];
                    vertices3[idx] = vertices[i + i2 + size * j];

                    ++idx;
                }    
            }

            size = midSize;

            SplitDataIn4Quad ( vertices0, regenerateFaces ( midSize, midSize ) );
            SplitDataIn4Quad ( vertices1, regenerateFaces ( midSize, midSize ) );
            SplitDataIn4Quad ( vertices2, regenerateFaces ( midSize, midSize ) );
            SplitDataIn4Quad ( vertices3, regenerateFaces ( midSize, midSize ) );
        } else {
            GenerateMesh ( vertices, faces, transform );
        }
    }


    Vector3[] applyPrecision ( Vector3[] vertices,int width, int height ) {
        List<Vector3> new_vertices = new List<Vector3> ( );
        for ( int j = 0; j < height; j+=precision ) {
            for ( int i = 0; i < width; i+=precision ) {
                new_vertices.Add ( vertices[i + j * (width)] );
            }
        }
        return new_vertices.ToArray ( );
    }


    int[] regenerateFaces ( int width, int height ) {
        List<int> facesIndex = new List<int> ( );

        for ( int j = 0; j < height - 1; j++ ) {
            for ( int i = 0; i < width - 1; i++ ) {
                int i0 = j * width + i;
                int i1 = i0 + width;

                facesIndex.Add ( i0 );
                facesIndex.Add ( i0 + 1 );
                facesIndex.Add ( i1 + 1 );

                facesIndex.Add ( i1 + 1 );
                facesIndex.Add ( i1 );
                facesIndex.Add ( i0 );
            }
        }

        return facesIndex.ToArray ( );
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


    Vector3[] getNormals ( float[] points ) {
        int pointsSize = points.Length;
        Vector3[] normals = new Vector3[pointsSize / 3];
        for ( int i = 0; i < pointsSize - 2; i += 3 ) {
            normals[i / 3] = new Vector3 ( points[i], points[i + 1], points[i + 2] );
        }
        return normals;
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
