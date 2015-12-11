using System;
using System.Runtime.InteropServices;

namespace TerrainCSharpLibrary {

    public class TerrainCSharpLibrary {
        // From c++ Dll (unmanaged)
        [DllImport ( "TerrainGen" )]
        public static extern float TestDll ( );

        [DllImport ( "TerrainGen" )]
        protected static extern void GenerateTerrainMesh ( 
            int width, 
            int height, 
            ref IntPtr vertex, ref int sizeVertex,
            ref IntPtr normals, ref int sizeNormals, 
            ref IntPtr faces, ref int sizeFaces );
        ///////////////////////////


        public static string TestWrapper ( ) {
            return "Wrapper func";
        }


        public static void GenerateTerrainData ( int width, int height, out float[] vertex, out float[] normals, out int[] faces ) {
            IntPtr vertex_ptr   = IntPtr.Zero;
            IntPtr normals_ptr  = IntPtr.Zero;
            IntPtr faces_ptr    = IntPtr.Zero;

            int sizeVertex  = 0;
            int sizeNormals = 0;
            int sizeFaces   = 0;

            GenerateTerrainMesh ( 
                width, 
                height, 
                ref vertex_ptr, ref sizeVertex,
                ref normals_ptr, ref sizeNormals,
                ref faces_ptr, ref sizeFaces );

            vertex  = new float[sizeVertex];
            normals = new float[sizeNormals];
            faces   = new int[sizeFaces];

            Marshal.Copy (
                vertex_ptr,
                vertex,
                0,
                sizeVertex );

            Marshal.Copy (
                normals_ptr,
                normals,
                0,
                sizeNormals );

            Marshal.Copy (
                faces_ptr,
                faces,
                0,
                sizeFaces );

        /*
          * WARNING!!!! IMPORTANT!!!
          * In this example the plugin created an array allocated
          * in unmanged memory.  The plugin will need to provide a
          * means to free the memory.
        */
        }

    }
}
