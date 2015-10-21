#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "../Graphics/Renderable.h"
#include "../Math/cVector.h"

namespace eae6320
{
	namespace Core
	{
		class GameObject
		{
		public:
			GameObject();
			bool Initialize(const char * i_FilePath);
			void Update();
			void ShutDown();
		public:
			Graphics::Renderable * Renderable;
			Math::cVector Position;
		};
	}
}

#endif //_GAMEOBJECT_H