using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System.IO;

public class Test : MonoBehaviour {

    // Straight From the c++ Dll (unmanaged)
    [DllImport ( "TerrainGen", EntryPoint = "TestDll" )]
    public static extern float DllTestFunc ( );


    void Start ( ) {
        float   res1    = DllTestFunc ( );
        string  res2    = TerrainCSharpLibrary.TerrainCSharpLibrary.TestWrapper ( );
        float   res3    = TerrainCSharpLibrary.TerrainCSharpLibrary.TestDll ( );

        Debug.Log ( res1 );
        Debug.Log ( res2 );
        Debug.Log ( res3 );
    }
	
}
