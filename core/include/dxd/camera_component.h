#pragma once

#include <DirectXMath.h>

#include "dxd/object.h"
#include "dxd/component.h"

namespace dxd
{

	class UCameraComponent : public UComponentImpl
	{
	public:
		~UCameraComponent() = default;

		UCameraComponent(UGameObject* GameObject);

		UCameraComponent(const UCameraComponent&) = delete;
		UCameraComponent(UCameraComponent&&) noexcept = default;

		UCameraComponent& operator=(const UCameraComponent&) = delete;
		UCameraComponent& operator=(UCameraComponent&&) noexcept = default;

		// TODO:
		// void LookAt(const DirectX::XMFLOAT3& TargetPosition, const DirectX::XMFLOAT3& UpVector = { 0.0f, 1.0f, 0.0f });

		void SetFovDegrees(float FoVDegrees);
		void SetAspectRatio(float AspectRatio);
		void SetNearPlane(float NearPlane);
		void SetFarPlane(float FarPlane);

		float GetFovDegrees();
		float GetAspectRatio();
		float GetNearPlane();
		float GetFarPlane();

		DirectX::XMMATRIX GetViewMatrix();
		DirectX::XMMATRIX GetProjectionMatrix();

	private:
		float FovDegrees;
		float AspectRatio;
		float NearPlane;
		float FarPlane;
	};

} // namespace dxd