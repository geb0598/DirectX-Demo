#include <stdexcept>

#include "dxd/camera_component.h"
#include "dxd/transform_component.h"

namespace dxd
{

	UCameraComponent::UCameraComponent(UGameObject* GameObject)
		: UComponentImpl(GameObject), FovDegrees(60.0f), AspectRatio(16.0f / 9.0f), NearPlane(0.1f), FarPlane(10.0f) {}

	void UCameraComponent::SetFovDegrees(float FovDegrees)
	{
		this->FovDegrees = FovDegrees;
	}

	void UCameraComponent::SetAspectRatio(float AspectRatio)
	{
		this->AspectRatio = AspectRatio;
	}

	void UCameraComponent::SetNearPlane(float NearPlane)
	{
		this->NearPlane = NearPlane;
	}

	void UCameraComponent::SetFarPlane(float FarPlane)
	{
		this->FarPlane = FarPlane;
	}

	float UCameraComponent::GetFovDegrees()
	{
		return FovDegrees;
	}

	float UCameraComponent::GetAspectRatio()
	{
		return AspectRatio;
	}

	float UCameraComponent::GetNearPlane()
	{
		return NearPlane;
	}

	float UCameraComponent::GetFarPlane()
	{
		return FarPlane;
	}

	DirectX::XMMATRIX UCameraComponent::GetViewMatrix()
	{
		auto Transform = GetGameObject()->GetComponent<UTransformComponent>();
		if (Transform == nullptr)
		{
			// TODO: return Unit Matrix?
			throw std::runtime_error("TODO");
		}
		
		DirectX::XMFLOAT3 Position = Transform->GetPosition();
		DirectX::XMFLOAT4 Rotation = Transform->GetRotation();

		DirectX::XMVECTOR PositionVector = DirectX::XMLoadFloat3(&Position);
		DirectX::XMVECTOR RotationVector = DirectX::XMLoadFloat4(&Rotation);

		DirectX::XMVECTOR ForwardVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		ForwardVector = DirectX::XMVector3Rotate(ForwardVector, RotationVector);

		DirectX::XMVECTOR TargetVector = DirectX::XMVectorAdd(PositionVector, ForwardVector);
		// TODO: What if UpVector and TargetVector are equal?
		DirectX::XMVECTOR UpVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		return DirectX::XMMatrixLookAtLH(PositionVector, TargetVector, UpVector);
	}

	DirectX::XMMATRIX UCameraComponent::GetProjectionMatrix()
	{
		float FovRadians = DirectX::XMConvertToRadians(FovDegrees);

		return DirectX::XMMatrixPerspectiveFovLH(FovRadians, AspectRatio, NearPlane, FarPlane);
	}

} // namespace dxd