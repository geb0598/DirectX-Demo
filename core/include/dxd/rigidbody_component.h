#pragma once

#include <DirectXMath.h>

#include "dxd/component.h"

namespace DXD
{

    class URigidbodyComponent : public UComponent
    {
    public:
        ~URigidbodyComponent() = default;

        URigidbodyComponent(UGameObject* GameObject, 
            DirectX::XMFLOAT3 Velocity = { 0.0f, 0.0f, 0.0f }, float Mass = 1.0f, float Restitution = 1.0f);

        void Update(float DeltaTime);

        void ApplyForce(const DirectX::XMFLOAT3& Force);
        void ApplyImpulse(const DirectX::XMFLOAT3& Impulse);

        float GetMass() const;
        float GetRestitution() const;
        bool IsGravityEnabled() const;
        DirectX::XMFLOAT3 GetVelocity() const;

        void SetMass(float Mass);
        void SetRestitution(float Restitution);
        void EnableGravity();
        void DisableGravity();
        void SetVelocity(const DirectX::XMFLOAT3& Velocity);

    private:
        static constexpr float GRAVITY = 9.81f;

        DirectX::XMFLOAT3 Velocity;
        DirectX::XMFLOAT3 Acceleration;
        DirectX::XMFLOAT3 Force;

        float Mass;
        float Restitution;

        bool bIsGravityEnabled;
    };

} // namespace DXD