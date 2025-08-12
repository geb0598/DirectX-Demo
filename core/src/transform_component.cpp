#include "dxd/component.h"
#include "dxd/transform_component.h"

namespace DXD
{

	UTransformComponent::UTransformComponent(UGameObject* GameObject) 
		: UComponent(GameObject), 
		  Position({ 0.0f, 0.0f, 0.0f }), Rotation({ 0.0f, 0.0f, 0.0f, 1.0f }), Scale({ 1.0f, 1.0f, 1.0f }) {}

	void UTransformComponent::Translate(const DirectX::XMFLOAT3& DeltaPosition)
	{
		DirectX::XMVECTOR PositionVector	  = DirectX::XMLoadFloat3(&Position);
		DirectX::XMVECTOR DeltaPositionVector = DirectX::XMLoadFloat3(&DeltaPosition);
		DirectX::XMVECTOR NewPositionVector	  = DirectX::XMVectorAdd(PositionVector, DeltaPositionVector);

		DirectX::XMStoreFloat3(&Position, NewPositionVector);
	}

	void UTransformComponent::TranslateLocal(const DirectX::XMFLOAT3& DeltaPosition)
	{
		DirectX::XMVECTOR LocalDeltaVector 	 = DirectX::XMLoadFloat3(&DeltaPosition);
		DirectX::XMVECTOR RotationQuaternion = DirectX::XMLoadFloat4(&Rotation);
		DirectX::XMVECTOR WorldDeltaVector 	 = DirectX::XMVector3Rotate(LocalDeltaVector, RotationQuaternion);

		DirectX::XMVECTOR PositionVector 	= DirectX::XMLoadFloat3(&Position);
		DirectX::XMVECTOR NewPositionVector = DirectX::XMVectorAdd(PositionVector, WorldDeltaVector);

		DirectX::XMStoreFloat3(&Position, NewPositionVector);
	}

	void UTransformComponent::Rotate(const DirectX::XMFLOAT4& Quaternion)
	{
		DirectX::XMVECTOR RotationVector	= DirectX::XMLoadFloat4(&Rotation);
		DirectX::XMVECTOR QuaternionVector  = DirectX::XMLoadFloat4(&Quaternion);
		DirectX::XMVECTOR NewRotationVector = DirectX::XMQuaternionMultiply(RotationVector, QuaternionVector);

		DirectX::XMStoreFloat4(&Rotation, NewRotationVector);
	}

	void UTransformComponent::Rotate(const DirectX::XMFLOAT3& RotationAxis, float AngleRadians)
	{
		DirectX::XMVECTOR RotationVector	 = DirectX::XMLoadFloat4(&Rotation);
		DirectX::XMVECTOR RotationAxisVector = DirectX::XMLoadFloat3(&RotationAxis);
		DirectX::XMVECTOR QuaternionVector   = DirectX::XMQuaternionRotationAxis(RotationAxisVector, AngleRadians);
		DirectX::XMVECTOR NewRotationVector  = DirectX::XMQuaternionMultiply(RotationVector, QuaternionVector);

		DirectX::XMStoreFloat4(&Rotation, NewRotationVector);
	}

	void UTransformComponent::Rotate(float Pitch, float Yaw, float Roll)
	{
		DirectX::XMVECTOR CurrentRotationQuaternion = DirectX::XMLoadFloat4(&Rotation);

		DirectX::XMVECTOR PitchQuaternion = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), Pitch);
		
		DirectX::XMVECTOR YawQuaternion = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), Yaw);

		DirectX::XMVECTOR RollQuaternion = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), Roll);

		DirectX::XMVECTOR NewRotationQuaternion = DirectX::XMQuaternionMultiply(YawQuaternion, CurrentRotationQuaternion);
		NewRotationQuaternion = DirectX::XMQuaternionMultiply(PitchQuaternion, NewRotationQuaternion);
		NewRotationQuaternion = DirectX::XMQuaternionMultiply(RollQuaternion, NewRotationQuaternion);

		NewRotationQuaternion = DirectX::XMQuaternionNormalize(NewRotationQuaternion);
		DirectX::XMStoreFloat4(&Rotation, NewRotationQuaternion);

		/*
		DirectX::XMVECTOR RotationVector = DirectX::XMLoadFloat4(&Rotation);
		DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);
		DirectX::XMVECTOR QuaternionVector = DirectX::XMQuaternionRotationMatrix(RotationMatrix);
		DirectX::XMVECTOR NewRotationVector = DirectX::XMQuaternionMultiply(RotationVector, QuaternionVector);

		DirectX::XMStoreFloat4(&Rotation, NewRotationVector);
		*/
	}
	
	void UTransformComponent::SetPosition(const DirectX::XMFLOAT3& Position)
	{
		this->Position = Position;
	}

	void UTransformComponent::SetPosition(const DirectX::XMFLOAT4& Position)
	{
		DirectX::XMVECTOR PositionVector = DirectX::XMLoadFloat4(&Position);
		DirectX::XMStoreFloat3(&this->Position, PositionVector);
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

	DirectX::XMFLOAT4 UTransformComponent::GetPosition4D() const
	{
		DirectX::XMVECTOR PositionVector = DirectX::XMLoadFloat3(&Position);
		PositionVector = DirectX::XMVectorSetW(PositionVector, 1.0f);
		DirectX::XMFLOAT4 Position4D;
		DirectX::XMStoreFloat4(&Position4D, PositionVector);
		return Position4D;
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
		DirectX::XMVECTOR ScaleVector	 = DirectX::XMLoadFloat3(&Scale);

		DirectX::XMMATRIX PositionMatrix = DirectX::XMMatrixTranslationFromVector(PositionVector);
		DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationQuaternion(RotationVector);
		DirectX::XMMATRIX ScaleMatrix	 = DirectX::XMMatrixScalingFromVector(ScaleVector);

		return ScaleMatrix * RotationMatrix * PositionMatrix;
	}

} // namespace DXD