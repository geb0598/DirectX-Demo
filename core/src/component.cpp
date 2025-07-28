#include "dxd/component.h"

namespace DXD
{

	UComponent::UComponent(UGameObject* GameObject) : GameObject(GameObject) {}

	UGameObject* UComponent::GetGameObject() 
	{
		return GameObject;
	}

	const UGameObject* UComponent::GetGameObject() const
	{
		return GameObject;
	}

} // namespace DXD