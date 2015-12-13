using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace TerrainCSharpLibrary {

    public class TerrainCSharpLibrary {

        // From c++ Dll (unmanaged)
        [DllImport ( "TerrainGen" )]
        public static extern int TestDll ( );

        [DllImport ( "TerrainGen" )]
        protected static extern void GenerateTerrainData ( 
            int width, 
            int height, 
            ref IntPtr vertex, ref int sizeVertex,
            ref IntPtr normals, ref int sizeNormals, 
            ref IntPtr faces, ref int sizeFaces,
            float precision );

        [DllImport ( "TerrainGen" )]
        protected static extern void GenerateTerrainMesh (
            int width, int height,
            string name, ref int meshCount,
            float precision );

        [DllImport ( "TerrainGen", CallingConvention = CallingConvention.StdCall )]
        protected static extern void GenerateTerrainMeshData (
            int width, int height,
            out IntPtr data,
            out int size,
            float precision );

        [DllImport ( "TerrainGen" )]
        protected static extern void GenerateTerrainHeights (
            int width,
            int height,
            ref IntPtr heights, ref float max );
        ///////////////////////////


        public static string TestWrapper ( ) {
            return "Wrapper foo";
        }


        public static void GenerateTerrainMeshData ( int width, int height, out string[] datas, float precision ) {
            IntPtr data_ptr = IntPtr.Zero;
            int size;

            GenerateTerrainMeshData ( width, height, out data_ptr, out size, precision );

            MarshalUnmananagedStrArray2ManagedStrArray ( data_ptr, size, out datas );          
        }


        // This method transforms an array of unmanaged character pointers (pointed to by pUnmanagedStringArray)
        // into an array of managed strings.
        //
        // This method also destroys each unmanaged character pointers and will also destroy the array itself.
        static void MarshalUnmananagedStrArray2ManagedStrArray
        (
          IntPtr pUnmanagedStringArray,
          int StringCount,
          out string[] ManagedStringArray
        ) {
            IntPtr[] pIntPtrArray = new IntPtr[StringCount];
            ManagedStringArray = new string[StringCount];

            Marshal.Copy ( pUnmanagedStringArray, pIntPtrArray, 0, StringCount );

            for ( int i = 0; i < StringCount; i++ ) {
                ManagedStringArray[i] = Marshal.PtrToStringAnsi ( pIntPtrArray[i] );
                Marshal.FreeCoTaskMem ( pIntPtrArray[i] );
            }

            Marshal.FreeCoTaskMem ( pUnmanagedStringArray );
        }



        public static void GenerateTerrainObjs ( int width, int height, string name, ref int meshCount, float precision ) {
            GenerateTerrainMesh ( width, height, name, ref meshCount, precision );
        }


        public static void GenerateTerrainHeights ( int width, int height, out float[] heights, ref float max ) {
            IntPtr heights_ptr = IntPtr.Zero;

            GenerateTerrainHeights ( width, height, ref heights_ptr, ref max );

            heights = new float[height * width];

            Marshal.Copy (
                heights_ptr,
                heights,
                0,
                height * width );

            Marshal.FreeCoTaskMem ( heights_ptr );
        }


        public static void GenerateTerrainHeightmap ( int width, int height, out float[,] heightmap, ref float max ) {
            IntPtr heights_ptr = IntPtr.Zero;

            GenerateTerrainHeights ( width, height, ref heights_ptr, ref max );

            heightmap = new float[height, width];

            float[] heights = new float[height * width];

            Marshal.Copy (
                heights_ptr,
                heights,
                0,
                height * width );

            Marshal.FreeCoTaskMem ( heights_ptr );

            int idx = 0;
            for ( int x = 0; x < width; ++x ) {
                for ( int y = 0; y < height; ++y ) {
                    heightmap[y, x] = heights[idx];
                    ++idx;
                }
            }
        }


        public static void GenerateTerrainData ( int width, int height, out float[] vertex, out float[] normals, out int[] faces, float precision ) {
            IntPtr vertex_ptr   = IntPtr.Zero;
            IntPtr normals_ptr  = IntPtr.Zero;
            IntPtr faces_ptr    = IntPtr.Zero;

            int sizeVertex  = 0;
            int sizeNormals = 0;
            int sizeFaces   = 0;

            GenerateTerrainData ( 
                width, 
                height, 
                ref vertex_ptr, ref sizeVertex,
                ref normals_ptr, ref sizeNormals,
                ref faces_ptr, ref sizeFaces,
                precision );

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

            Marshal.FreeCoTaskMem ( vertex_ptr );
            Marshal.FreeCoTaskMem ( normals_ptr );
            Marshal.FreeCoTaskMem ( faces_ptr );
        }

    }
}
