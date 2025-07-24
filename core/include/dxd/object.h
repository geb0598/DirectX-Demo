#pragma once

#include <memory>
#include <vector>
#include <type_traits>

#include "component.h"
#include "transform_component.h"

namespace dxd
{

	class UGameObject
	{
	public:
		~UGameObject() = default;

		UGameObject();

		UGameObject(const UGameObject&) = delete;
		UGameObject(UGameObject&&) noexcept = default;

		UGameObject& operator=(const UGameObject&) = delete;
		UGameObject& operator=(UGameObject&&) noexcept = default;

		template<typename T, typename... Args> 
		std::enable_if_t<std::is_base_of_v<IComponent, T>>
		AddComponent(Args&&... args)
		{
			// TODO: Check duplicated components
			Components.push_back(std::make_unique<T>(this, std::forward<Args>(args)...));
		}

		template<typename T>
		std::enable_if_t<std::is_base_of_v<IComponent, T>, T>*
		GetComponent()
		{
			for (const auto& Component : Components)
			{	// TODO: RTTI overhead
				//       To mitigate the issue, use enum class, uid, etc.
				if (T* TargetComponent = dynamic_cast<T*>(Component.get()))
				{
					return TargetComponent;
				}
			}
			return nullptr;
		}

	private:
		std::vector<std::unique_ptr<IComponent>> Components;
	};

} // namespace dxd
