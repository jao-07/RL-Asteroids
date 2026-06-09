//
// Created by Lucas N. Ferreira on 12/09/23.
//

#pragma once
#include "Actor.h"

class Laser : public Actor
{
public:
    Laser(class Game* game, float length = 10.0f, float deathTimer = 0.25f);

    void OnUpdate(float deltaTime) override;

private:
    float mLength;
    float mDeathTimer;

    class DrawComponent* mDrawComponent;
    class RigidBodyComponent* mRigidBodyComponent;
    class CircleColliderComponent* mCircleColliderComponent;
};
