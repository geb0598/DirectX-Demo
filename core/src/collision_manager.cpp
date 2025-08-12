#include "dxd/collision_manager.h"
#include "dxd/collider_component.h"
#include "dxd/rigidbody_component.h"

namespace DXD
{

    void UCollisionManager::CheckCollisions()
    {
        for (size_t i = 0; i < GameObjects.size(); ++i) 
        {
            for (size_t j = i + 1; j < GameObjects.size(); ++j)
            {
                auto Collider1 = GameObjects[i]->GetComponent<DXD::USphereColliderComponent>();
                auto Collider2 = GameObjects[j]->GetComponent<DXD::USphereColliderComponent>();

                if (Collider1 && Collider2 && Collider1->CheckCollision(*Collider2))
                {
                    auto Rigidbody1 = GameObjects[i]->GetComponent<DXD::URigidbodyComponent>();
                    auto Rigidbody2 = GameObjects[j]->GetComponent<DXD::URigidbodyComponent>();

                    if (Rigidbody1 && Rigidbody2)
                    {
                        ResolveCollision(Collider1, Collider2, Rigidbody1, Rigidbody2);
                    }
                }
            }
        }
    }

    void UCollisionManager::ResolveCollision(
        USphereColliderComponent* Collider1, USphereColliderComponent* Collider2,
        URigidbodyComponent* Rigidbody1, URigidbodyComponent* Rigidbody2)
    {
        
    }
}