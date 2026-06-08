//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Ship.h"
#include "Asteroid.h"
#include "../Game.h"
#include "../Actors/Laser.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponent.h"

Ship::Ship(Game* game,
           const float height,
           const float forwardForce,
           const float rotationForce,
           const float frictionCoefficient)
        : Actor(game)
        , mLaserCooldown(0.f)
        , mHeight(height)
        , mRigidBodyComponent(nullptr)
        , mDrawComponent(nullptr)
        , mForwardSpeed(forwardForce)
        , mRotationForce(rotationForce)
        , mFrictionCoefficient(frictionCoefficient)
{
    Vector2 vert1 = Vector2(-mHeight/2, mHeight/1.5);
    Vector2 vert2 = Vector2(mHeight, 0);
    Vector2 vert3 = Vector2(-mHeight/2, -mHeight/1.5);
    Vector2 vert4 = Vector2(-mHeight/4, 0);

    std::vector<Vector2> verts;
    verts.push_back(vert1);
    verts.push_back(vert2);
    verts.push_back(vert3);

    mDrawComponent = new DrawComponent(this, verts);
    mRigidBodyComponent = new RigidBodyComponent(this);
    mCircleColliderComponent = new CircleColliderComponent(this, mHeight/2);
}

void Ship::Reset() {
    mRigidBodyComponent->SetAcceleration(Vector2(0, 0));
    mRigidBodyComponent->SetVelocity(Vector2(0, 0));
    mRigidBodyComponent->SetAngularSpeed(0);
    SetPosition(Vector2(mGame->GetWindowWidth() / 2.0f, mGame->GetWindowHeight() / 2.0f));
}

void Ship::OnProcessInput(const uint8_t* state)
{

    if (state[SDL_SCANCODE_W]){
        Vector2 forward = GetForward();
        mRigidBodyComponent->ApplyForce(forward * mForwardSpeed);
    }

    float angularSpeed = 0;
    if (state[SDL_SCANCODE_A]) {
        angularSpeed -= mRotationForce;
    }

    if (state[SDL_SCANCODE_D]) {
        angularSpeed += mRotationForce;
    }

    if ((state[SDL_SCANCODE_SPACE]) && (mLaserCooldown <= 0.0f)) {
        Laser* l = new Laser(this->mGame, 5);
        l->SetPosition(this->mPosition + GetForward() * mHeight);
        l->SetRotation(this->mRotation);
        l->GetComponent<RigidBodyComponent>()->ApplyForce(GetForward() * 3000.f);
        mLaserCooldown = 0.5;
    }

    mRigidBodyComponent->SetAngularSpeed(angularSpeed);
}

void Ship::OnUpdate(float deltaTime)
{
    // mLaserCooldown -= deltaTime;
    mLaserCooldown = mLaserCooldown - deltaTime <= 0.0f ? 0.0f : mLaserCooldown - deltaTime;

    Vector2 velocity = mRigidBodyComponent->GetVelocity();

    if(velocity.Length() > 0) {
        Vector2 vNorm = velocity;
        vNorm.Normalize();
        vNorm = Vector2::Zero - vNorm;
        float speedSquared = velocity.LengthSq();
        Vector2 drag = vNorm * speedSquared * mFrictionCoefficient;

        mRigidBodyComponent->ApplyForce(drag);
    }

    std::vector<Asteroid*> asteroids = mGame->GetAsteroids();
    for (auto & asteroid : asteroids) {
        if (asteroid != nullptr) {
            auto *ast = asteroid->GetComponent<CircleColliderComponent>();
            if (this->mCircleColliderComponent->Intersect(*ast)) {
                mIsDead = true;
            }
        }
    }
}
