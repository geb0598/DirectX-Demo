#pragma once

#include <DirectXMath.h>

#include "dxd/component.h"

namespace DXD
{

	struct FCameraComponentDesc
	{
		float FovDegree	  = 60.0f;
		float AspectRatio = 16.0f / 9.0f;
		float NearPlane   = 0.1f;
		float FarPlane    = 10.0f;;
	};

	class UCameraComponent : public UComponent
	{
	public:
		~UCameraComponent() = default;

		UCameraComponent(UGameObject* GameObject, FCameraComponentDesc CameraComponentDesc = {});

		UCameraComponent(const UCameraComponent&) = delete;
		UCameraComponent& operator=(const UCameraComponent&) = delete;

		UCameraComponent(UCameraComponent&&) noexcept = default;
		UCameraComponent& operator=(UCameraComponent&&) noexcept = default;

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

} // namespace DXD