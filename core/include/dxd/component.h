#pragma once

namespace dxd
{

	class UGameObject;

	class IComponent
	{
	public:
		virtual ~IComponent() = default;

		virtual UGameObject* GetGameObject() const = 0;
	};

	class UComponentImpl : public IComponent
	{
	public:
		virtual ~UComponentImpl() = default;

		UComponentImpl(const UComponentImpl&) = delete;
		UComponentImpl(UComponentImpl&&) noexcept = default;

		UComponentImpl& operator=(const UComponentImpl&) = delete;
		UComponentImpl& operator=(UComponentImpl&&) noexcept = default;

		explicit UComponentImpl(UGameObject* GameObject);

		virtual UGameObject* GetGameObject() const override;

	private:
		UGameObject* GameObject;
	};

} // namespace dxd
