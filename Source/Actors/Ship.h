//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Actor.h"

class Ship : public Actor
{
public:
    explicit Ship(Game* game, float height,
                              float forwardForce = 500.0f,
                              float rotationForce = 5.0f,
                              float frictionCoefficient = 0.005f);

    void OnProcessInput(Action action) override;
    void OnUpdate(float deltaTime) override;
    void Reset();

private:
    float mForwardSpeed;
    float mRotationForce;
    float mFrictionCoefficient;
    float mLaserCooldown;
    float mHeight;

    class DrawComponent* mDrawComponent;
    class RigidBodyComponent* mRigidBodyComponent;
    class CircleColliderComponent* mCircleColliderComponent;
};