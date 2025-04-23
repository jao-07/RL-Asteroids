//
// Created by joaov on 23/04/2025.
//

#pragma once
#include "Actor.h"
#include "../Game.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponent.h"


class Particle : public Actor{
    public:
        Particle(Game* game, Vector2 position, Vector2 velocity, float deathTimer = 1.0f);

        void OnUpdate(float deltaTime) override;

    private:
        float mDeathTimer;
        RigidBodyComponent* mRigidBodyComponent;
        DrawComponent* mDraw;
};

