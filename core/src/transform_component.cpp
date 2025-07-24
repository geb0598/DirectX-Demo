#include "dxd/transform_component.h"

namespace dxd
{

	UTransformComponent::UTransformComponent(UGameObject* GameObject) 
		: UComponentImpl(GameObject), 
		  Position({ 0.0f, 0.0f, 0.0f }), Rotation({ 0.0f, 0.0f, 0.0f, 1.0f }), Scale({ 1.0f, 1.0f, 1.0f }) {}

	void UTransformComponent::Translate(const DirectX::XMFLOAT3& DeltaPosition)
	{
		DirectX::XMVECTOR PositionVector = DirectX::XMLoadFloat3(&Position);
		DirectX::XMVECTOR DeltaPositionVector = DirectX::XMLoadFloat3(&DeltaPosition);
		DirectX::XMVECTOR NewPositionVector = DirectX::XMVectorAdd(PositionVector, DeltaPositionVector);

		DirectX::XMStoreFloat3(&Position, NewPositionVector);
	}

	void UTransformComponent::Rotate(const DirectX::XMFLOAT4& Quaternion)
	{
		DirectX::XMVECTOR RotationVector = DirectX::XMLoadFloat4(&Rotation);
		DirectX::XMVECTOR QuaternionVector = DirectX::XMLoadFloat4(&Quaternion);
		DirectX::XMVECTOR NewRotationVector = DirectX::XMQuaternionMultiply(RotationVector, QuaternionVector);

		DirectX::XMStoreFloat4(&Rotation, NewRotationVector);
	}

	void UTransformComponent::Rotate(const DirectX::XMFLOAT3& RotationAxis, float AngleRadians)
	{
		DirectX::XMVECTOR RotationAxisVector = DirectX::XMLoadFloat3(&RotationAxis);
		DirectX::XMVECTOR NewRotationVector = DirectX::XMQuaternionRotationAxis(RotationAxisVector, AngleRadians);

		DirectX::XMStoreFloat4(&Rotation, NewRotationVector);
	}
	
	void UTransformComponent::SetPosition(const DirectX::XMFLOAT3& Position)
	{
		this->Position = Position;
	}

	void UTransformComponent::SetRotation(const DirectX::XMFLOAT4& Rotation)
	{
		this->Rotation = Rotation;
	}

	void UTransformComponent::SetScale(const DirectX::XMFLOAT3& Scale)
	{
		this->Scale = Scale;
	}
	DirectX::XMFLOAT3 UTransformComponent::GetPosition() const
	{
		return Position;
	}

	DirectX::XMFLOAT4 UTransformComponent::GetRotation() const
	{
		return Rotation;
	}

	DirectX::XMFLOAT3 UTransformComponent::GetScale() const
	{
		return Scale;
	}

	DirectX::XMMATRIX UTransformComponent::GetWorldMatrix() const
	{
		DirectX::XMVECTOR PositionVector = DirectX::XMLoadFloat3(&Position);
		DirectX::XMVECTOR RotationVector = DirectX::XMLoadFloat4(&Rotation);
		DirectX::XMVECTOR ScaleVector = DirectX::XMLoadFloat3(&Scale);

		DirectX::XMMATRIX PositionMatrix = DirectX::XMMatrixTranslationFromVector(PositionVector);
		DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationQuaternion(RotationVector);
		DirectX::XMMATRIX ScaleMatrix = DirectX::XMMatrixScalingFromVector(ScaleVector);

		// TODO: DirectX uses row-major, but HLSL uses column-major?
		return DirectX::XMMatrixTranspose(ScaleMatrix * RotationMatrix * PositionMatrix);
	}

} // namespace dxd