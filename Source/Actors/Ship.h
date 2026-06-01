//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Actor.h"

class Ship : public Actor
{
public:
    explicit Ship(Game* game, float height,
                              float forwardForce = 800.0f,
                              float rotationForce = 3.0f,
                              float frictionCoefficient = 0.2f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void Reset();

    bool GetIsDead() {return mIsDead;}

private:
    float mForwardSpeed;
    float mRotationForce;
    float mFrictionCoefficient;
    float mLaserCooldown;
    float mHeight;

    bool mIsDead = false;

    class DrawComponent* mDrawComponent;
    class RigidBodyComponent* mRigidBodyComponent;
    class CircleColliderComponent* mCircleColliderComponent;
};