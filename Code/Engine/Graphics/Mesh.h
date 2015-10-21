#ifndef EAE6320_MESH_H
#define EAE6320_MESH_H

#include <cstdint>

#if defined EAE6320_PLATFORM_GL
#include "../../Externals/OpenGlExtensions/OpenGlExtensions.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#elif defined EAE6320_PLATFORM_D3D
#include <d3d9.h>
#include <d3dx9shader.h>
#endif //Platform Check

namespace eae6320
{
	namespace Graphics
	{
		struct sVertex
		{
			// POSITION
			// 2 floats == 8 bytes
			// Offset = 0
			float x, y;
			// COLOR0
			// 4 uint8_ts == 4 bytes
			// Offset = 8
#if defined EAE6320_PLATFORM_GL
			uint8_t r, g, b, a;	// 8 bits [0,255] per RGBA channel (the alpha channel is unused but is present so that color uses a full 4 bytes)
#elif defined EAE6320_PLATFORM_D3D
			uint8_t b, g, r, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
#endif //Platform Check
		};



		class Mesh
		{
			uint32_t mVertexCount, mIndexCount;
			sVertex * mVertexData;
			uint32_t * mIndexData;


#if defined EAE6320_PLATFORM_GL
			// A vertex array encapsulates both the vertex and index data as well as the vertex format
			GLuint s_vertexArrayId /*= 0*/;
#elif defined EAE6320_PLATFORM_D3D
			
			IDirect3DVertexDeclaration9* s_vertexDeclaration = NULL;
			// The vertex buffer holds the data for each vertex
			IDirect3DVertexBuffer9* s_vertexBuffer = NULL;
			// An index buffer describes how to make triangles with the vertices
			// (i.e. it defines the vertex connectivity)
			IDirect3DIndexBuffer9* s_indexBuffer = NULL;
#endif // Platform Check
		



		//Methods
			
		public:
			Mesh();
			//static Mesh * CreateMesh();
			bool Initialize(void * buffer);
			void Draw();
			bool ShutDown();

			void * LoadMesh(const char * i_path);

#if defined EAE6320_PLATFORM_GL
			bool CreateVertexArray();
#elif defined EAE6320_PLATFORM_D3D
			static IDirect3DDevice9* s_direct3dDevice;
			static void SetDirect3dDevice(IDirect3DDevice9* i_direct3dDevice);
			static void ReleaseDirect3dDevice();
			bool CreateVertexBuffer();
			bool CreateIndexBuffer();
#endif //Platform Check
		};
	}
}

#endif // EAE6320_MESH_H
