#pragma once

#include <vector>

#include "dxd/game_object.h"
#include "dxd/collider_component.h"
#include "dxd/rigidbody_component.h"

namespace DXD
{

    class UCollisionManager
    {
    public:
        ~UCollisionManager() = default;

        UCollisionManager() = default;

        void CheckCollisions();

    private:
        void ResolveCollision(USphereColliderComponent* Collider1, USphereColliderComponent* Collider2,
            URigidbodyComponent* Rigidbody1, URigidbodyComponent* Rigidbody2);

        // TODO: Dangling Pointer Problem
        std::vector<DXD::UGameObject*> GameObjects;
    };

} // namespace DXD