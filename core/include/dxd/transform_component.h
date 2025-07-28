#pragma once

#include <DirectXMath.h>

#include "dxd/component.h"

namespace DXD
{

	class UTransformComponent : public UComponent
	{
	public:
		~UTransformComponent() = default;

		UTransformComponent(UGameObject* GameObject);

		UTransformComponent(const UTransformComponent&) = delete;
		UTransformComponent(UTransformComponent&&) noexcept = default;

		UTransformComponent& operator=(const UTransformComponent&) = delete;
		UTransformComponent& operator=(UTransformComponent&&) noexcept = default;

		void Translate(const DirectX::XMFLOAT3& DeltaPosition);
		void Rotate(const DirectX::XMFLOAT4& Quaternion);
		void Rotate(const DirectX::XMFLOAT3& RotationAxis, float AngleRadians);

		void SetPosition(const DirectX::XMFLOAT3& Position);
		void SetPosition(const DirectX::XMFLOAT4& Position);
		void SetRotation(const DirectX::XMFLOAT4& Rotation);
		void SetScale(const DirectX::XMFLOAT3& Scale);

		DirectX::XMFLOAT3 GetPosition() const;
		DirectX::XMFLOAT4 GetPosition4D() const;
		DirectX::XMFLOAT4 GetRotation() const; 
		DirectX::XMFLOAT3 GetScale() const;

		DirectX::XMMATRIX GetWorldMatrix() const;

	private:
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Rotation;
		DirectX::XMFLOAT3 Scale;
	};

} // namespace DXD
