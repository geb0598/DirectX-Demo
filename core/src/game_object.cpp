#include "dxd/game_object.h"

#include "dxd/component.h"
#include "dxd/transform_component.h"

namespace DXD
{

	UGameObject::UGameObject() 
	{
		// NOTE: Transform Component is required for all GameObjects
		AddComponent<UTransformComponent>();
	}

} // namespace DXD