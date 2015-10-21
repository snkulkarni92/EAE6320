#ifndef EAE6320_CMESHBUILDER_H
#define EAE6320_CMESHBUILDER_H

// Header Files
//=============

#include "../BuilderHelper/cbBuilder.h"

#include "../../Externals/Lua/Includes.h"

// Class Declaration
//==================

namespace eae6320
{
	class cMeshBuilder : public cbBuilder
	{
	public:
		struct sVertex
		{
			float x, y;
#if defined EAE6320_PLATFORM_GL
			uint8_t r, g, b, a;
#elif defined EAE6320_PLATFORM_D3D
			uint8_t b, g, r, a;
#endif
		};
	private:
		uint32_t mVertexCount, mIndexCount;
		sVertex * mVertexData;
		uint32_t * mIndexData;

		// Interface
		//==========

	public:

		// Build
		//------

		virtual bool Build(const std::vector<std::string>& i_arguments);

		bool ProcessMeshData(lua_State& io_luaState);
	};
}

#endif	// EAE6320_CMESHBUILDER_H
