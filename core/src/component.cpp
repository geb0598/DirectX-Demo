#include "dxd/component.h"

namespace dxd
{

	UComponentImpl::UComponentImpl(UObject* Object) : Object(Object) {}

	UObject* UComponentImpl::GetObject() const
	{
		return Object;
	}

} // namespace dxd