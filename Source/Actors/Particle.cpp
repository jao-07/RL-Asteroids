//
// Created by joaov on 23/04/2025.
//

#include "Particle.h"

Particle::Particle(Game* game, Vector2 position, Vector2 velocity, float deathTimer)
    : Actor(game)
    , mDeathTimer(deathTimer)
{
    SetPosition(position);

    // Desenho simples: talvez só um ponto ou uma linha
    std::vector<Vector2> verts;
    verts.emplace_back(Vector2(0.0f, 0.0f));
    verts.emplace_back(Vector2(1.0f, 1.0f)); // um risquinho
    mDraw = new DrawComponent(this, verts);

    mRigidBodyComponent = new RigidBodyComponent(this, 0.1f);
    mRigidBodyComponent->ApplyForce(velocity);
}

void Particle::OnUpdate(float deltaTime) {
    mDeathTimer -= deltaTime;
    if (mDeathTimer <= 0.0f) {
        SetState(ActorState::Destroy);
    }
}



