using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace CityGen {

    public class CityCSharpLibrary {

        // From c++ Dll (unmanaged)
        [DllImport ( "CityGen" )]
        public static extern int TestDll ( );

        [DllImport ( "CityGen" )]
        protected static extern void GenerateCityData (
            out int meshcount,
            out IntPtr vertices, out IntPtr vertexSizes,
            out IntPtr faces, out IntPtr faceSizes );
        ///////////////////////////


        public static string TestWrapper ( ) {
            return "Wrapper foo";
        }


        public static void GenerateCity ( out int meshCount, out float[] vertices, out int[] verticeSizes, out int[] faces, out int[] faceSize ) {
            IntPtr vertices_ptr = IntPtr.Zero;
            IntPtr vsizes_ptr   = IntPtr.Zero;
            IntPtr faces_ptr    = IntPtr.Zero;
            IntPtr fsizes_ptr   = IntPtr.Zero;

            GenerateCityData ( out meshCount, out vertices_ptr, out vsizes_ptr, out faces_ptr, out fsizes_ptr );
            
            //////
            verticeSizes = new int[meshCount];
            Marshal.Copy ( vsizes_ptr, verticeSizes, 0, meshCount );

            int totalSize = 0;
            for ( int i = 0; i < meshCount; ++i ) {
                totalSize += verticeSizes[i];
            }

            vertices = new float[totalSize];
            Marshal.Copy ( vertices_ptr, vertices, 0, totalSize );

            //////
            faceSize = new int[meshCount];
            Marshal.Copy ( fsizes_ptr, faceSize, 0, meshCount );

            totalSize = 0;
            for ( int i = 0; i < meshCount; ++i ) {
                totalSize += faceSize[i];
            }

            faces = new int[totalSize];
            Marshal.Copy ( fsizes_ptr, faces, 0, totalSize );

            //////
            Marshal.FreeCoTaskMem ( vertices_ptr );
            Marshal.FreeCoTaskMem ( vsizes_ptr );
            Marshal.FreeCoTaskMem ( faces_ptr );
            Marshal.FreeCoTaskMem ( fsizes_ptr );

            System.GC.Collect ( );
        }

    }
}
