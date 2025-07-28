#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "dxd/component.h"

namespace DXD
{

	class UGameObject
	{
	public:
		~UGameObject() = default;

		UGameObject();

		UGameObject(const UGameObject&) = delete;
		UGameObject& operator=(const UGameObject&) = delete;

		UGameObject(UGameObject&&) noexcept = default;
		UGameObject& operator=(UGameObject&&) noexcept = default;

		template<typename TComponent, typename... TArgs> 
		std::enable_if_t<std::is_base_of_v<IComponent, TComponent>>
		AddComponent(TArgs&&... Args);

		template<typename TComponent>
		std::enable_if_t<std::is_base_of_v<IComponent, TComponent>, TComponent>*
		GetComponent();

	private:
		// TODO: Use std::map to manage Components
		std::vector<std::unique_ptr<IComponent>> Components;
	};

	// -------------------------------------------------------------------------------- //
	//							Implmentations and Details								//
	// -------------------------------------------------------------------------------- //

	template<typename TComponent, typename... TArgs>
	std::enable_if_t<std::is_base_of_v<IComponent, TComponent>>
	UGameObject::AddComponent(TArgs&&... Args)
	{
		// TODO: Check duplicated components
		Components.push_back(std::make_unique<TComponent>(this, std::forward<TArgs>(Args)...));
	}

	template<typename TComponent>
	std::enable_if_t<std::is_base_of_v<IComponent, TComponent>, TComponent>*
	UGameObject::GetComponent()
	{
		for (const auto& Component : Components)
		{	// TODO: RTTI overhead(to mitigate the issue, use enum class, uid, etc)
			if (TComponent* TargetComponent = dynamic_cast<TComponent*>(Component.get()))
			{
				return TargetComponent;
			}
		}
		return nullptr;
	}

} // namespace DXD
