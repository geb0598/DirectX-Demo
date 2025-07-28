#pragma once

namespace DXD
{

	// NOTE: Forward declaration of UGameObject class
	class UGameObject;

	class IComponent
	{
	public:
		virtual ~IComponent() = default;

		virtual UGameObject* GetGameObject() = 0;
		virtual const UGameObject* GetGameObject() const = 0;
	};

	class UComponent : public IComponent
	{
	public:
		virtual ~UComponent() = default;

		explicit UComponent(UGameObject* GameObject);

		UComponent(const UComponent&) = delete;
		UComponent& operator=(const UComponent&) = delete;

		UComponent(UComponent&&) noexcept = default;
		UComponent& operator=(UComponent&&) noexcept = default;

		virtual UGameObject* GetGameObject() override;
		virtual const UGameObject* GetGameObject() const override;

	private:
		UGameObject* GameObject;
	};

} // namespace DXD
