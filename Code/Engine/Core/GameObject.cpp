#include "GameObject.h"

eae6320::Core::GameObject::GameObject()
{
	Renderable = new eae6320::Graphics::Renderable();
	Position.x = 0.0f;
	Position.y = 0.0f;
}

bool eae6320::Core::GameObject::Initialize(const char * i_FilePath)
{
	if (!this->Renderable->Initialize(i_FilePath))
	{
		ShutDown();
		return false;
	}
	return true;
}

void eae6320::Core::GameObject::Update()
{
	Renderable->SetPositionOffset(Position);
}

void eae6320::Core::GameObject::ShutDown()
{
	if (Renderable)
	{
		Renderable->ShutDown();
		delete Renderable;
	}
}