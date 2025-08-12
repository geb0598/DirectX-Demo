#include "dxd/collider_component.h"

namespace DXD
{
    USphereColliderComponent::USphereColliderComponent(UGameObject* GameObject, 
        const DirectX::XMFLOAT3& Center, float Radius)
        : UComponent(GameObject), Center(Center), Radius(Radius), bIsCollisionEnabled(true) {}

    // void USphereColliderComponent::Update(float DeltaTime) {}

    bool USphereColliderComponent::CheckCollision(const USphereColliderComponent& Other) const
    {
        if (!IsCollisionEnabled() || !Other.IsCollisionEnabled())
        {
            return false;
        }

        float RadiusSum = Radius + Other.Radius;

        DirectX::XMVECTOR CenterVector      = DirectX::XMLoadFloat3(&Center);
        DirectX::XMVECTOR OtherCenterVector = DirectX::XMLoadFloat3(&Other.Center);
        DirectX::XMVECTOR DiffVector        = DirectX::XMVectorSubtract(CenterVector, OtherCenterVector);
        DirectX::XMVECTOR DistVector        = DirectX::XMVector3Length(DiffVector);

        float Distance = 0.0f;
        DirectX::XMStoreFloat(&Distance, DistVector);

        return Distance <= RadiusSum;
    }

    DirectX::XMFLOAT3 USphereColliderComponent::GetCenter() const 
    {
        return Center;
    }

    float USphereColliderComponent::GetRadius() const
    {
        return Radius;
    }

    bool USphereColliderComponent::IsCollisionEnabled() const 
    {
        return bIsCollisionEnabled;
    }

    void USphereColliderComponent::SetCenter(const DirectX::XMFLOAT3& Center)
    {
        this->Center = Center;
    }

    void USphereColliderComponent::SetRadius(float Radius) 
    {
        this->Radius = Radius;
    }

    void USphereColliderComponent::EnableCollision()
    {
        bIsCollisionEnabled = true;
    }

    void USphereColliderComponent::DisableCollision()
    {
        bIsCollisionEnabled = false;
    }

}