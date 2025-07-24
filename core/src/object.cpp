#include "dxd/object.h"

namespace dxd
{

	UGameObject::UGameObject() 
	{
		// NOTE: Transform Component is required for all GameObjects
		AddComponent<UTransformComponent>();
	}

} // namespace dxd