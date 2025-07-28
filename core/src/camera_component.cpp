#include <stdexcept>

#include "dxd/game_object.h"
#include "dxd/component.h"
#include "dxd/camera_component.h"
#include "dxd/transform_component.h"

namespace DXD
{

	UCameraComponent::UCameraComponent(UGameObject* GameObject, FCameraComponentDesc CameraComponentDesc)
		: UComponent(GameObject), 
		  FovDegrees(CameraComponentDesc.FovDegree), 
		  AspectRatio(CameraComponentDesc.AspectRatio), 
		  NearPlane(CameraComponentDesc.NearPlane), 
		  FarPlane(CameraComponentDesc.FarPlane) {}

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
			throw std::runtime_error("Cannot find Transform component");
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

} // namespace DXD