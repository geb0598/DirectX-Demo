#pragma once

namespace dxd
{

	class UObject;

	class IComponent
	{
	public:
		virtual ~IComponent() = default;

		virtual UObject* GetObject() const = 0;
	};

	class UComponentImpl : public IComponent
	{
	public:
		virtual ~UComponentImpl() = default;

		explicit UComponentImpl(UObject* Object);

		virtual UObject* GetObject() const override;

	private:
		UObject* Object;
	};

} // namespace dxd
