//
// Created by Lucas N. Ferreira on 12/09/23.
//

#include "Laser.h"
#include "Asteroid.h"
#include "../Game.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponent.h"

Laser::Laser(Game* game, const float length, const float deathTimer)
        :Actor(game)
        ,mLength(length)
        ,mDeathTimer(deathTimer)
{
    Vector2 v1 = Vector2(-mLength/2, 0);
    Vector2 v2 = Vector2(mLength/2, 0);

    std::vector<Vector2> verts;
    verts.push_back(v1);
    verts.push_back(v2);

    mDrawComponent = new DrawComponent(this, verts);
    mRigidBodyComponent = new RigidBodyComponent(this, 0.1);
    mCircleColliderComponent = new CircleColliderComponent(this, mLength);
}

void Laser::OnUpdate(float deltaTime)
{
    mDeathTimer -= deltaTime;
    if(mDeathTimer <= 0) {
        SetState(ActorState::Destroy);
        mGame->SetLasersMissed(true);
    }

    const std::vector<Asteroid*> Asteroids = mGame->GetAsteroids();
    for (auto Asteroid : Asteroids) {
        if (auto *ast = Asteroid->GetComponent<CircleColliderComponent>(); this->mCircleColliderComponent->Intersect(*ast)) {
            //SDL_Log("Asteroid (%f, %f) hit", Asteroids[i]->GetPosition().x, Asteroids[i]->GetPosition().y);
            SetState(ActorState::Destroy);
            Asteroid->SetState(ActorState::Destroy);
            break;
        }
    }
}
