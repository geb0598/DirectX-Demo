#pragma once

#include <DirectXMath.h>

#include "dxd/component.h"

namespace DXD
{

    // NOTE: Interface is required for further expansion
    class USphereColliderComponent : public UComponent
    {
    public:
        ~USphereColliderComponent() = default;
        USphereColliderComponent(UGameObject* GameObject, const DirectX::XMFLOAT3& Center = { 0.0f, 0.0f, 0.0f }, float Radius = 1.0f);

        // void Update(float DeltaTime);

        bool CheckCollision(const USphereColliderComponent& Other) const;

        DirectX::XMFLOAT3 GetCenter() const;
        float GetRadius() const;
        bool IsCollisionEnabled() const;

        void SetCenter(const DirectX::XMFLOAT3& Center);
        void SetRadius(float Radius);
        void EnableCollision();
        void DisableCollision();

    private:
        // NOTE: Object Space Position
        DirectX::XMFLOAT3 Center;
        float Radius;

        bool bIsCollisionEnabled;

        // TODO: Mesh for debugging
    };

} // namespace DXD