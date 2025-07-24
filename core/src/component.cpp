#include "dxd/component.h"

namespace dxd
{

	UComponentImpl::UComponentImpl(UGameObject* GameObject) : GameObject(GameObject) {}

	UGameObject* UComponentImpl::GetGameObject() const
	{
		return GameObject;
	}

} // namespace dxd