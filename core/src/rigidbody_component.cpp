#include "dxd/rigidbody_component.h"
#include "dxd/transform_component.h"
#include "dxd/game_object.h"
#include "dxd/rigidbody_component.h"

namespace DXD
{

    URigidbodyComponent::URigidbodyComponent(UGameObject* GameObject, 
        DirectX::XMFLOAT3 Velocity, float Mass, float Restitution)
        : UComponent(GameObject),
          Velocity(Velocity), Mass(Mass), Restitution(Restitution), 
          Acceleration({ 0.0f, 0.0f, 0.0f }), 
          bIsGravityEnabled(false) {}

    void URigidbodyComponent::Update(float DeltaTime)
    {
        if (IsGravityEnabled())
        {
            ApplyForce({ 0.0f, -GRAVITY * Mass, 0.0f});
        }

        // NOTE: Mass should not become zero
        assert(Mass != 0);
        Acceleration.x = Force.x / Mass;
        Acceleration.y = Force.y / Mass;
        Acceleration.z = Force.z / Mass;

        Velocity.x += Acceleration.x * DeltaTime;
        Velocity.y += Acceleration.y * DeltaTime;
        Velocity.z += Acceleration.z * DeltaTime;

        DirectX::XMVECTOR VelocityVector = DirectX::XMLoadFloat3(&Velocity);
        VelocityVector = DirectX::XMVectorScale(VelocityVector, DeltaTime);
        DirectX::XMStoreFloat3(&Velocity, VelocityVector);

        auto Transform = GetGameObject()->GetComponent<DXD::UTransformComponent>();
        Transform->Translate(Velocity);

        Force = { 0.0f, 0.0f, 0.0f };
    }

    void URigidbodyComponent::ApplyForce(const DirectX::XMFLOAT3& Force)
    {
        this->Force.x += Force.x;
        this->Force.y += Force.y;
        this->Force.z += Force.z;
    }

    void URigidbodyComponent::ApplyImpulse(const DirectX::XMFLOAT3& Impulse)
    {
        // NOTE: Mass should not become zero
        assert(Mass != 0);

        Velocity.x += Impulse.x / Mass;
        Velocity.y += Impulse.y / Mass;
        Velocity.z += Impulse.z / Mass;
    }

    float URigidbodyComponent::GetMass() const
    {
        return Mass;
    }

    float URigidbodyComponent::GetRestitution() const
    {
        return Restitution;
    }

    bool URigidbodyComponent::IsGravityEnabled() const
    {
        return bIsGravityEnabled;
    }

    DirectX::XMFLOAT3 URigidbodyComponent::GetVelocity() const
    {
        return Velocity;
    }

    void URigidbodyComponent::SetMass(float Mass)
    {
        this->Mass = Mass;
    }

    void URigidbodyComponent::SetRestitution(float Restitution)
    {
        this->Restitution = Restitution;
    }

    void URigidbodyComponent::EnableGravity()
    {
        bIsGravityEnabled = true;
    }

    void URigidbodyComponent::DisableGravity()
    {
        bIsGravityEnabled = false;
    }

    void URigidbodyComponent::SetVelocity(const DirectX::XMFLOAT3 &Velocity)
    {
        this->Velocity = Velocity;
    }

} // namespace DXD