#ifndef _RENDERABLE_H
#define _RENDERABLE_H

#include "Mesh.h"
#include "Effect.h"
#include "../Math/cVector.h"
namespace eae6320
{
	namespace Graphics
	{
		class Renderable
		{
		public:
			Renderable();
			bool Initialize(const char * i_FilePath);
			void Draw();
			void ShutDown();

			void SetPositionOffset(Math::cVector i_Offset);
		public:
			Mesh * Mesh;
			Effect * Effect;
			Math::cVector Offset;
		};
	}
}
#endif //_RENDERABLE_H